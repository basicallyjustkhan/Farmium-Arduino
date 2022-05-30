#include "DHT.h"            // DHT Library (Adafruit)
#include <SoftwareSerial.h> // Bluetooth Module HM10

SoftwareSerial bluetooth(1, 0); // Rx, Tx Pins (Rx - Rx; Tx - Tx)

const int   dry = 1000.00;  // If the soil is dryer (>=) than this number
const float cold = 24.00;   // If the soil is colder (<=) than this number
const float hot = 27.00;    // If the soil is hotter (>=) than this number

const int fc28 = 13;        // FC-28 Pin (Soil Sensor)
const int buzzer = 12;      // Buzzer Pin
const int coolingFan = 11;  // Cooling Fan Pin
const int heatingFan = 10;  // Heating Fan Pin
const int pump = A0;        // Water Pump Pin
#define dht22 A1            // DHT22 Pin (Temp Sensor)

const int red_light   = A2;  // R Pin
const int green_light = A3;  // G Pin
const int blue_light  = A4;  // B Pin

#define DHTTYPE DHT22       // Defining the type of the sensor
DHT dht(dht22, DHTTYPE);    // Letting the library know


void setup() 
{
  // Setting the Pin modes
  pinMode(fc28, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(coolingFan, OUTPUT);
  pinMode(heatingFan, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(dht22, INPUT);
  pinMode(red_light, OUTPUT);
  pinMode(green_light, OUTPUT);
  pinMode(blue_light, OUTPUT);
  
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
  checkBluetooth(moisture, humidity, temperature);          // Bluetooth Controls
  checkPump(moisture, dry, pump);                           // Water Pump Controls
  checkFan(temperature, hot, cold, coolingFan, heatingFan); // Fan Controls
}

// Bluetooth Function
void checkBluetooth(int moisture, int humidity, int temperature)
{
  bluetooth.println(moisture);
  bluetooth.println(humidity);
  bluetooth.println(temperature);
}

// Pump Function
void checkPump(int moisture, int dry, const int pump)
{
  if (moisture >= dry)
  {
    // Turn on the Pump
    RGB_color(255, 0, 0); // Red
    digitalWrite(pump, LOW);
  }
  else
  {
    RGB_color(0, 255, 0); // Green
    digitalWrite(pump, HIGH);
  }
}

// Fan Function
void checkFan(int temperature, int hot, int cold, const int coolingFan, const int heatingFan)
{
  if (temperature >= hot)
  {
    // Turn on the Cooling Fan
    RGB_color(255, 0, 0); // Red
    digitalWrite(coolingFan, HIGH);
  }
  else if (temperature <= cold)
  {
    // Turn on the Heating Fan
    RGB_color(255, 0, 0); // Red
    digitalWrite(heatingFan, HIGH);
  }
  else
  {
    RGB_color(0, 255, 0); // Green
    digitalWrite(coolingFan, LOW);
    digitalWrite(heatingFan, LOW);
  }
}

// RGB Function
void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(red_light, red_light_value);
  analogWrite(green_light, green_light_value);
  analogWrite(blue_light, blue_light_value);
}
