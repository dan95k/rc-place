#include <SoftwareSerial.h>
#include "EBYTE.h"
#include <Servo.h>

#define TX  3
#define RX  2
#define M0  13
#define M1  12
#define AUX  11

unsigned long time;

Servo servo1;  // create servo object to control a servo
Servo servo2;  // create servo object to control a servo
Servo servo3;  // create servo object to control a servo
Servo servo4;  // create servo object to control a servo
Servo esc;  // create servo object to control a servo
 
struct DATA {
  int j1posY;
  int j1posX;
  int j2posY;
  int j2posX;
  boolean button1;
  boolean button2;
  boolean button3;
  boolean button4;
};

int Chan,escValue;
DATA MyData;

SoftwareSerial ESerial(TX, RX);

EBYTE Transceiver(&ESerial, M0, M1, AUX);

void setup() {

  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(AUX, INPUT);

  servo1.attach(4);  // attaches the servo on pin 4 to the servo object
  servo2.attach(5);  // attaches the servo on pin 5 to the servo object
  servo3.attach(6);  // attaches the servo on pin 6 to the servo object
  servo4.attach(7);  // attaches the servo on pin 7 to the servo object
  esc.attach(8);  // attaches the servo on pin 7 to the servo object
  resetData();

  Serial.begin(9600);
  ESerial.begin(9600);
  Serial.println("Starting Reader");

  Transceiver.init();
  
  // Transceiver.Reset();
  // Transceiver.SetAirDataRate(ADR_8K);
  //Transceiver.SetAddressH(0);
  //Transceiver.SetAddressL(0);
  //Transceiver.SetChannel(2);
  // Transceiver.SaveParameters(PERMANENT);
   //Transceiver.PrintParameters();

}

void loop() {

  if (ESerial.available()) {

    Transceiver.GetStruct(&MyData, sizeof(MyData));

       servo1.write(MyData.j1posX); //need to reverse it or the lower one
       servo2.write(MyData.j1posX); 
       servo3.write(MyData.j2posY);
       servo4.write(MyData.j2posX);
       escValue = map(MyData.j1posY, 0, 180, 0, 2000);
       esc.writeMicroseconds(escValue);
       delay(10);
  }
  else {
    if (Serial.available()) {
      Chan = Serial.read();
      Serial.println(Chan);
      if (Chan > 47) {
        Serial.println(Chan - 48);
        Transceiver.SetChannel(Chan - 48);
        Transceiver.SaveParameters(PERMANENT);
        Transceiver.PrintParameters();
      }
    }
    delay(10);
    //Serial.println("Searching: ");
  }
}

void resetData() {
  // Reset the values when there is no radio connection - Set initial default values
  MyData.j1posY = 90;
  MyData.j1posX = 90;
  MyData.j2posY = 90;
  MyData.j2posX = 90;
  MyData.button1 = false;
  MyData.button2 = false;
  MyData.button3 = false;
  MyData.button4 = false;
}
