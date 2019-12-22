#include <SoftwareSerial.h>
#include "EBYTE.h"

#define TX  3
#define RX  2
#define M0  13
#define M1  12
#define AUX  11
#define communicationLed 8

int Chan;
unsigned long time;

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

DATA MyData;

SoftwareSerial ESerial(TX, RX);

EBYTE Transceiver(&ESerial, M0, M1, AUX);

void setup() {

  Serial.begin(9600);
  ESerial.begin(9600);
  Serial.println("Starting Sender");
  Transceiver.init();
  pinMode(4, INPUT_PULLUP); //button pull-up resistor
  pinMode(5, INPUT_PULLUP); //button pull-up resistor
  pinMode(6, INPUT_PULLUP); //button pull-up resistor
  pinMode(7, INPUT_PULLUP); //button pull-up resistor
  pinMode(communicationLed, OUTPUT); //communication Led config
  // Transceiver.SetTransmitPower(OPT_TP20); UDR_115200 try this later
  // Transceiver.SetAirDataRate(ADR_8K);
  //Transceiver.SetAddressH(0);
  //Transceiver.SetAddressL(0);
  //Transceiver.SetChannel(2);
  //Transceiver.SaveParameters(PERMANENT);
  // Transceiver.PrintParameters();
  // SetWORTIming(OPT_WAKEUP250);

}

void loop() {

  MyData.j1posX = map(analogRead(A0), 0, 1023, 0, 180); // Convert the analog read value from 0 to 1023 into values from 0 to 180
  MyData.j1posY = map(analogRead(A1), 0, 1023, 0, 180);
  MyData.j2posY = map(analogRead(A2), 0, 1023, 0, 180);
  MyData.j2posX = map(analogRead(A3), 0, 1023, 0, 180);
  if (digitalRead(4)) MyData.button1 = false; else MyData.button1= true;
  if (digitalRead(5)) MyData.button2 = false; else MyData.button2= true;
  if (digitalRead(6)) MyData.button3 = false; else MyData.button3= true;
  if (digitalRead(7)) MyData.button4 = false; else MyData.button4= true;

  Transceiver.SendStruct(&MyData, sizeof(MyData));
  //delay(1000);
  
    Serial.print("Time: ");
  time = millis();
  Serial.println(time);
  
  if (Serial.available()) {
    Chan = Serial.read();
    if (Chan > 47) {
      Serial.println(Chan-48);
      Transceiver.SetChannel(Chan-48);
      Transceiver.SaveParameters(PERMANENT);
      Transceiver.PrintParameters();
    }
  }

}
