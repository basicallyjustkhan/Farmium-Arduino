#include "DHT.h"
#include <SoftwareSerial.h>

SoftwareSerial bluetooth(1, 0); // Rx, Tx

const int   dry = 1000.00;         // if the soil is dryer than this number, then start watering
const float hot = 25.50;           // if the soil is hotter than this number, then start the fan

const int red_light   = 8;
const int green_light = 7;
const int blue_light  = 6;

int bluetoothLogic = 0;
const int soilSensor = 13;
const int buzzer = 12;
const int fans = 11;
const int pump = A0;
#define dht22 A1

#define DHTTYPE DHT22 
DHT dht(dht22, DHTTYPE);


void setup() 
{
  pinMode(red_light, OUTPUT);
  pinMode(green_light, OUTPUT);
  pinMode(blue_light, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(fans, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(soilSensor, INPUT);
  pinMode(dht22, INPUT);
  

  digitalWrite(pump, HIGH);
  RGB_color(255, 255, 255); // White
  bluetooth.begin(9600);
  dht.begin();
  
  digitalWrite(buzzer, HIGH);
  delay(150);
  digitalWrite(buzzer, LOW);

  //bluetooth.println("Started!");

  //bluetooth.println("AT+NAME=FarmiumMiniPrototype");
  //bluetooth.println("AT+PSWD=1092");
}

void loop() 
{
  // Get Data
  float moisture = 100-((analogRead(soilSensor))/10);
  float humidity = dht.readHumidity();
  int temperature = dht.readTemperature();
  float hic = dht.computeHeatIndex(temperature, humidity, false);
  delay(1000);
  
  // Bluetooth 
  checkBluetooth(moisture, humidity, temperature);

  // Plant Controls
  checkPump(moisture, dry, pump);
  checkFan(temperature, hot, fans);
}

void checkBluetooth(int moisture, int humidity, int temperature) // Bluetooth
{
  bluetooth.println(moisture);
  bluetooth.println(humidity);
  bluetooth.println(temperature);
}

void checkPump(int moisture, int dry, int pump) // Pump controls
{
  if (moisture >= dry or bluetoothLogic == 1) // 0-off; 1-on
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

void checkFan(int temperature, int hot, int fans) // Fan controls
{
  if (temperature >= hot or bluetoothLogic == 2) // 0-off; 2-on
  {
    while (temperature >= hot)
    {
      // the plant is too hot
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

void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(red_light, red_light_value);
  analogWrite(green_light, green_light_value);
  analogWrite(blue_light, blue_light_value);
}
