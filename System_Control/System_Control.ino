/*Code to control heating and inflation subsystems
(Unfinished)*/

//********************pin defination********************
#define  INFLATE_CONTROL_PIN  22 
#define  TEMP_CONTROL_PIN     23
#define  TEMP_SENSOR_PIN      A0
#define  DIR_PIN              32
#define  IDIR_PIN             33

//*****************constants defination*****************
#define   T1    2000 //the data read from sensor pin when temperature is 40C, 3000 is randomly chosen, need to test
#define   T2    3000 //50C

int  INFLATE_TIME = 2000; //2s to inflate,2s to deflate,2s is randomly chosen, should be controlled through like an app
int HEAT_MODE = 0; //should be controlled through app (0 for no heating, 1 and 2 for heating to different temperature)
int TEMP = 0; //current temperature

int time_begin = millis();
bool dir = false; //true for inflation, false for deflation


//**********************pin setup *********************
void pinSetup(){
    pinMode(INFLATE_CONTROL_PIN,OUTPUT);
    pinMode(TEMP_CONTROL_PIN ,OUTPUT);
    pinMode(DIR_PIN,OUTPUT);
    pinMode(IDIR_PIN,OUTPUT);
    pinMode(TEMP_SENSOR_PIN,INPUT);
  
}

//*******************WiFi or bluetooth ****************
void WiFicontrol(){
    //to be filled in
}



//*******************heating subsystem ****************
void heating(){
    TEMP = analogRead(TEMP_SENSOR_PIN); //read current temperature from temperature sensor
    if(HEAT_MODE == 0) digitalWrite(TEMP_CONTROL_PIN,LOW);
    else if(HEAT_MODE == 1) {
        if(TEMP < T1) digitalWrite(TEMP_CONTROL_PIN,HIGH);
        else digitalWrite(TEMP_CONTROL_PIN,LOW);
    }
    else if(HEAT_MODE == 2) {
        if(TEMP < T2) digitalWrite(TEMP_CONTROL_PIN,HIGH);
        else digitalWrite(TEMP_CONTROL_PIN,LOW);
    }
    
}


//*******************pressure subsystem ****************
void inflation(){
    if(millis() - time_begin > INFLATE_TIME){
      time_begin = millis();
      dir = !dir;
    }
    if(dir){
      digitalWrite(DIR_PIN,HIGH);
      digitalWrite(IDIR_PIN,LOW);
    }else {
      digitalWrite(DIR_PIN,LOW);
      digitalWrite(IDIR_PIN,HIGH);
    }
}

//**********************main function *******************
void setup() {
    pinSetup();
    WiFicontrol();
}

void loop() {
    heating();
    inflation();

}
