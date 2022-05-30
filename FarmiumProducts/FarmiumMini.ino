#include "DHT.h"            // DHT Library (Adafruit)
#include <SoftwareSerial.h> // Bluetooth Module Library

SoftwareSerial bluetooth(1, 0); // Rx, Tx Pins

const int   dry = 1000.00;  // If the soil is dryer than this number
const float hot = 25.50;    // If the soil is hotter than this number

const int red_light   = 8;  // R Pin
const int green_light = 7;  // G Pin
const int blue_light  = 6;  // B Pin

const int fc28 = 13;        // FC-28 Pin (Soil Sensor)
const int buzzer = 12;      // Buzzer Pin
const int fans = 11;        // Fans Pin
const int pump = A0;        // Water Pump Pin
#define dht22 A1            // DHT22 Pin (Temp Sensor)

#define DHTTYPE DHT22       // Defining the type of the sensor
DHT dht(dht22, DHTTYPE);    // Letting the library know


void setup() 
{
  // Setting the Pin modes
  pinMode(red_light, OUTPUT);
  pinMode(green_light, OUTPUT);
  pinMode(blue_light, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(fans, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(fc28, INPUT);
  pinMode(dht22, INPUT);
  
  // Setting Initial Values
  digitalWrite(pump, HIGH); // Relay
  RGB_color(255, 255, 255); // White
  
  // Connection Parameters
  bluetooth.begin(9600);                    // CMD Language
  bluetooth.println("Started!");            // For Tester Usage
  bluetooth.println("AT+NAME=FarmiumMini"); // Naming the Bluetooth
  bluetooth.println("AT+PSWD=1092");        // Any 4 Digit Password
  dht.begin();

  // Buzzer Start-Up Noise
  digitalWrite(buzzer, HIGH);
  delay(150);                               // Beep Length (ms)
  digitalWrite(buzzer, LOW); 
}

void loop() 
{
  // Get Data
  float moisture = 100-((analogRead(fc28))/10); // Moisture Readings
  float humidity = dht.readHumidity();          // Humidity Readings
  int temperature = dht.readTemperature();      // Temperature Readings
  float hic = dht.computeHeatIndex(temperature, humidity, false); // Heat Index Readings
  delay(1000);  // Delay between readings due to the Bluetooth Module
  
  // Controls 
  checkBluetooth(moisture, humidity, temperature);  // Bluetooth Controls
  checkPump(moisture, dry, pump);                   // Water Pump Controls
  checkFan(temperature, hot, fans);                 // Fan Controls
}

// Bluetooth Function
void checkBluetooth(int moisture, int humidity, int temperature)
{
  bluetooth.println(moisture);
  bluetooth.println(humidity);
  bluetooth.println(temperature);
}

// Pump Function
void checkPump(int moisture, int dry, int pump)
{
  if (moisture >= dry)
  {
    while (true)
    {
      // Turn on the Pump
      RGB_color(255, 0, 0); // Red
      digitalWrite(pump, LOW);
      break;
    }
  }
  else
  {
    RGB_color(0, 255, 0); // Green
    digitalWrite(pump, HIGH);
  }
}

// Fan Function
void checkFan(int temperature, int hot, int fans)
{
  if (temperature >= hot)
  {
    while (temperature >= hot)
    {
      // Turn on the 
      RGB_color(255, 0, 0); // Red
      digitalWrite(fans, HIGH);
      break;
    }
  }
  else
  {
    RGB_color(0, 255, 0); // Green
    digitalWrite(fans, LOW);
  }
}

// RGB Function
void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(red_light, red_light_value);
  analogWrite(green_light, green_light_value);
  analogWrite(blue_light, blue_light_value);
}
