// Sketch to get inverter status

#include "SoftwareSerial.h"
#include "inverter.h"

SoftwareSerial conn(7, 4, true); // setup serial connection with inverter. With my SunEzy 2801 inverse logic is needed.
Inverter inv(conn); // create Inverter object
String layout;

void setup() {
    Serial.begin(9600);
    conn.begin(9600); // open connection to inverter
    inv.reset(); // reset communication with inverter
    String sn = inv.discover();
    bool ok = inv.begin(sn); // begin communication with inverter
    layout = inv.statLayout(); // ask for status layout
}

void loop() {
    InverterStatus status; // create a new status object
    inv.status(status, layout); // ask for status, passing the layout
    Serial.println(status.pac);
    delay(1000);
}
