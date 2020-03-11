/*
 * For microcontroller on the device
 * Pump on/off, solenoid valve on/off controlled by two switches 
 * Microcontroller: ESP32
 */
 
#define  PUMP_PIN   22 
#define  VALVE_PIN  23

#include <WiFi.h>
#include <WiFiUDP.h>

const char* ssid = "Crampr_Dampr", * pass = "cramprdampr";          // WiFi name and password
IPAddress IPlocal(192,168,1,179), IPtarget(192,168,1,171);    // WiFi address
WiFiUDP udp;                                                  // WiFi UDP
unsigned int UDPlocalPort = 2816, UDPtargetPort = 2918;       // WiFi port
const int packetSize = 20;                                   // packet size  
byte receive_buffer[packetSize];                              // array to store received buffer
int SW_1 = 0, SW_2 = 0;            //data received  


void STA_UDP_Set(){       //set UDP to STA mode
    // Wifi indication
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on
    delay(1000);
    
    Serial.print("Connecting to: "); Serial.println(ssid);  // connect to WiFi   
    WiFi.begin(ssid, pass);  // connect to network with password
    IPAddress gateway(192,168,1,1);             // init gateway IP
    IPAddress subnet(255,255,255,0);            // init subnet mask
    WiFi.config(IPlocal, gateway, subnet);      // set IP address of ESP
    udp.begin(UDPlocalPort);    // configure a port for UDP comms
    // hold the code here and wait until the WiFi is connected
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected!"); 
    
    //Turn OFF to indicate WiFi connected successfully
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
}



void UDPreceiveData(){
    if (udp.parsePacket())
    {
        udp.read(receive_buffer, packetSize);

        // receive data from controller
        SW_1 = receive_buffer[0];
        SW_2 = receive_buffer[1];
       // Serial.print("I'm not dead!");
}
}


void setup() {
    Serial.begin(115200);
    pinMode(PUMP_PIN,OUTPUT);
    pinMode(VALVE_PIN,OUTPUT);
    STA_UDP_Set();
}

void loop() {
    UDPreceiveData();
    if(SW_1 == 2) digitalWrite(PUMP_PIN,HIGH);
    else digitalWrite(PUMP_PIN,LOW);
    if(SW_2 == 2) digitalWrite(VALVE_PIN,HIGH);
    else digitalWrite(VALVE_PIN,LOW);
    Serial.print(SW_1);
    Serial.print("  ");
    Serial.println(SW_2);
    delay(1);
}
