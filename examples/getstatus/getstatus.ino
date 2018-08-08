// Sketch to get inverter status

#include "SoftwareSerial.h"
#include "inverter.h"

SoftwareSerial conn(7, 4, true); // setup serial connection with inverter. With my SunEzy 2801 inverse logic is needed.
Inverter inv(&conn); // create Inverter object
char buf[MAX_PLOAD_SIZE]; // initialize a buffer to store inverter responses
uint8_t layoutLen; // length of the status layout

void setup() {
  Serial.begin(9600);
  conn.begin(9600); // open connection to inverter
  inv.reset(); // reset communication with inverter
  inv.discover(buf); // get available inverter(s)
  bool ok = inv.begin(buf); // begin communication with inverter
  layoutLen = inv.statLayout(buf); // ask for status layout, store the result in buf
}

void loop() {
  // put your main code here, to run repeatedly:
  InverterStatus status; // create a new status object
  inv.status(&status, buf, layoutLen); // ask for status, passing the layout (stored in buf) and the layout length
  Serial.println(status.pac);
  delay(1000);
}


