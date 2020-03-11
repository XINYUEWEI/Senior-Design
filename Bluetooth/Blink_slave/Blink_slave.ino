/*
 * On the  controller 
 *  == SLAVE ==
 */

#define ButtonPin 7
int state = 0;

void setup() {
    pinMode(ButtonPin, INPUT);
    pinMode(4,OUTPUT);
    //pinMode(0,OUTPUT);
   // Serial1.begin(38400);
    Serial.begin(9600);
}

void loop() {
   // Serial.println(Serial1.available());
    state = digitalRead(ButtonPin);
    //Serial1.write(state);
//    digitalWrite(0,HIGH);
//    delay(10);
//    digitalWrite(0,LOW);
//    delay(10);
//    delay(20);
    if(state == 1) Serial.println('1'); //print
    else Serial.println('0');
}
