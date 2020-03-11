/*
 * On the belt
 *  == MASTER ==
 */

#define LedPin 7
int state = 0;

void setup() {
    pinMode(LedPin, OUTPUT);
    digitalWrite(LedPin, LOW);
    Serial1.begin(38400);
    Serial.begin(38400);

}

void loop() {
    state = Serial1.read();
    Serial.println(state);
    //Serial.println(Serial1.available());
    if(Serial1.available()){
      digitalWrite(LedPin, HIGH);
      state = Serial1.read();
      if(state == 0) digitalWrite(LedPin, LOW);
      else digitalWrite(LedPin, HIGH);
//      Serial.println(state);
    }

}
