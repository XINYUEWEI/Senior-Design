//slave
#define button 7
#include <SoftwareSerial.h>
SoftwareSerial BT(11,10); //RX|TX
int state = 0;

void setup() {
    pinMode(button, INPUT);
    Serial.begin(38400);
    BT.begin(38400);
}

void loop() {
    Serial.println(BT.available());
    state = digitalRead(button);
    //Serial.println(state);
    if (state == 1) BT.write('1');
    else BT.write('0');
    delay(10);
}
