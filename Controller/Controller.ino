/*
 * For microcontroller on the remote controller
 * Pump on/off, solenoid valve on/off controlled by two switches 
 * Microcontroller: ESP32
 */
 
#include <WiFi.h>
#include <WiFiUDP.h>

#define SW_pin_1 23 // switch 1
#define SW_pin_2 22 // switch 2

// WiFi name
const char* ssid = "Crampr_Dampr";                // this is the network name
const char* pass = "cramprdampr";         // this is the password

// IP Addresses
IPAddress IPlocal(192,168,1,171);          // initialize local IP address
IPAddress IPtarget(192,168,1,179);         // initialize target IP address

// variables for UDP
WiFiUDP udp;
unsigned int UDPlocalPort = 2918;   // UDP port number for local ESP
unsigned int UDPtargetPort = 2816;  // UDP port number on the target ESP
const int packetSize = 20;         // define packetSize (length of message)
byte sendBuffer[packetSize];        // create the sendBuffer array
byte receiveBuffer[packetSize];     // create the receiveBuffer array

// send five data
int SW_1 = 0;     // switch 1 data
int SW_2 = 0;     // switch 2 data


void AP_UDP_Set(){          // UDP AP set up
    // Wifi indication
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);
       
    WiFi.mode(WIFI_AP);         // sets the WiFi mode to be AP mode
    WiFi.softAP(ssid, pass); // configures the ESP in AP mode with network name and password
    delay(100);                 // hack need to wait 100ms for AP_START...
    Serial.print("Set softAPConfig "); Serial.println(ssid);    // debug statement
    IPAddress gateway(192,168,1,1);                 // initializes gateway IP
    IPAddress subnet(255,255,255,0);                // initializes subnet mask
    WiFi.softAPConfig(IPlocal, gateway, subnet);    // sets the IP addr of ESP to IPlocal
    udp.begin(UDPlocalPort);    // configure a port for UDP comms
    IPAddress myIP = WiFi.softAPIP();   // demo the usage of the softAPIP command
    Serial.print("AP IP address: "); Serial.println(myIP);      // debug statement

    //Turn OFF to indicate WiFi setup successfully
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
}


void UDPsendData(){         // send data
    // split the integer
    sendBuffer[0] = SW_1;             // send SW_1
    sendBuffer[1] = SW_2;             // send SW_2
    sendBuffer[2] = 0;                // null terminate string

    // send the message
    udp.beginPacket(IPtarget, UDPtargetPort);   // target IP and target port num to send info to
    udp.printf("%s", sendBuffer);               // send the contents of sendBuffer over WiFiUDP
    udp.endPacket();
    // end message
}


void setup() {
    Serial.begin(115200);
    pinMode(SW_pin_1, INPUT);
    pinMode(SW_pin_2, INPUT);
    AP_UDP_Set();
    
}

void loop() {
    SW_1 = digitalRead(SW_pin_1) + 1;             // switch 1 data transfromed to {1,2}
    SW_2 = digitalRead(SW_pin_2) + 1;             // switch 2 data transfromed to {1,2}
    UDPsendData();        // send data
//    Serial.print(SW_1);
//    Serial.print("  ");
//    Serial.println(SW_2);
//    delay(1);
    
}
