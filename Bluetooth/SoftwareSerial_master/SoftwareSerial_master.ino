//master
#define led 7
#include <SoftwareSerial.h>
SoftwareSerial BT(11,10); //RX|TX
int state = 0;

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(38400);
  BT.begin(38400);

}

void loop() {
  Serial.println(BT.available());
  if(BT.available()){
    state = BT.read();
    Serial.println(state);
    delay(10);
  }

}
