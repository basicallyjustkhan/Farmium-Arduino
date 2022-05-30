#include <SoftwareSerial.h>

//Pins  ************************
//UART TO HM10 Module
const uint8_t bluRX_ardTXpin = 9;
const uint8_t bluTX_ardRXpin = 10;

//RGB LED
const uint8_t redPin = 9;
const uint8_t greenPin = 10;
const uint8_t bluePin = 11;
// Push Button
const uint8_t pushButtonPin = 6;

SoftwareSerial bluetooth(bluTX_ardRXpin, bluRX_ardTXpin);

//function prototypes
void checkBluetooth();

void setup() {
  bluetooth.begin(9600);
  Serial.begin(9600);
  pinMode(pushButtonPin, INPUT_PULLUP);
}

void loop() {

  checkBluetooth();//if something is coming at us


  if (!digitalRead(pushButtonPin)) {//send out
    bluetooth.print("Push Button");
    while (!digitalRead(pushButtonPin)) {}
    delay(20);
    bluetooth.print(" ");
  }


}

void checkBluetooth() {
  char charBuffer[20];//most we would ever see
  if (bluetooth.available() > 0) {
    int numberOfBytesReceived = bluetooth.readBytesUntil('\n', charBuffer, 19);
    charBuffer[numberOfBytesReceived] = NULL;
    //    Serial.print("Received: ");
    //    Serial.println(charBuffer);

    if (strstr(charBuffer, "LED OFF") == &charBuffer[0]) {
      Serial.println("Turning LED OFF");
      analogWrite(redPin, 255);
      analogWrite(greenPin, 255);
      analogWrite(bluePin, 255);

    } else if (strstr(charBuffer, "LED Color#") == &charBuffer[0]) {
      char hexColorString[10] = "";
      strcpy(hexColorString, &charBuffer[strlen("LED Color#")]);
      unsigned long colorInt = 0;
      //http://www.asciitable.com
      for (int i = 0; i < 6; i++) {
        if (hexColorString[i] < 97) { //number parts
          colorInt = colorInt + (((long)hexColorString[i] - 48) << ((5 - i) * 4));
        } else { //letter in hex parts
          colorInt = colorInt + (((long)hexColorString[i] - 97  + 10) << ((5 - i) * 4));
        }
      }

      //Serial.println(colorInt);
      byte redColor = colorInt >> (16);

      analogWrite(redPin, ~redColor);

      byte greenColor = colorInt >> (8);
      analogWrite(greenPin, ~greenColor);
      //Serial.println(greenColor);
      byte blueColor = colorInt;
      analogWrite(bluePin, ~blueColor);

      Serial.print(redColor);
      Serial.print(",");
      Serial.print(greenColor);
      Serial.print(",");
      Serial.println(blueColor);
    }
  }
}
