/*
 * SENIOR DESIGN PROJECT
 * TEAM: CRAMPR DAMPR
 * BlE ESP32
 * For ESP32 on the belt
 */


#include "BLEDevice.h"

#define   PUMP_PIN          21 
#define   VALVE_PIN_1       22
#define   VALVE_PIN_2       23
#define   HEAT_PIN          32
#define   TEMP_SENSOR_PIN   33

//inflation levels 
int Inflation_Period1[2] = {5000,5000}; //infalte | deflate (unit:ms)
int Inflation_Period2[2] = {10000,10000};
int Inflation_Period3[2] = {15000,15000};
int inflation_start_time = 0;

//heating levels
int Heat_Period1[2] = {100000,1000000}; //heat | no heat (unit:ms)
int Heat_Period2[2] = {200000,1000000};
int Heat_Period3[2] = {300000,1000000};
int Safe_Temp = 60;
int heat_start_time = 0;

//the initial state received from the remote controller
std::string SW_state = "0000"; 
std::string Inflate_SW = "00";
std::string Heat_SW = "00";



//------------------------------------ Bluetooth --------------------------------------
// The remote service we wish to connect to.
static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;
static BLEClient* pClient;
static BLERemoteService* pRemoteService;

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
    Serial.print("data: ");
    Serial.println((char*)pData);
}

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

bool clientInit(){
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    pClient = BLEDevice::createClient();
    Serial.println(" - Created client");
    pClient->setClientCallbacks(new MyClientCallback());
    pClient->connect(myDevice);
    Serial.println(" - Connected to server");
    pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
        Serial.print("Failed to find our service UUID: ");
        Serial.println(serviceUUID.toString().c_str());
        pClient->disconnect();
        return false;
    }
    Serial.println(" - Found our service");
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
        Serial.print("Failed to find our characteristic UUID: ");
        Serial.println(charUUID.toString().c_str());
        pClient->disconnect();
        return false;
    }
    Serial.println(" - Found our characteristic");
}

bool clientReceive(){
    if(pRemoteCharacteristic->canRead()) {
        SW_state = pRemoteCharacteristic->readValue();
        Serial.println(SW_state.c_str());
    }

    if(pRemoteCharacteristic->canNotify())
        pRemoteCharacteristic->registerForNotify(notifyCallback);
    connected = true;
}

/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.print("BLE Advertised Device found: ");
      Serial.println(advertisedDevice.toString().c_str());

      // We have found a device, let us now see if it contains the service we are looking for.
      if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

        BLEDevice::getScan()->stop();
        myDevice = new BLEAdvertisedDevice(advertisedDevice);
        doConnect = true;
        doScan = true;
    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks

void BLEClientSetup(){
    BLEDevice::init("");
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(5, false);

    clientInit();
}


//------------------------------------ Inflation --------------------------------------
void InflationCycle(int period[2]){
    int cur_inflation_time = millis();
    digitalWrite(PUMP_PIN,HIGH);
    if((cur_inflation_time - inflation_start_time) < period[0]){
      digitalWrite(VALVE_PIN_1, HIGH);
      digitalWrite(VALVE_PIN_2, LOW);
    }
    else if((cur_inflation_time - inflation_start_time) < (period[0] + period[1])){
      digitalWrite(VALVE_PIN_1, LOW);
      digitalWrite(VALVE_PIN_2, HIGH);
    }
    else inflation_start_time = cur_inflation_time;
}


//------------------------------------ Heating --------------------------------------
void HeatCycle(int period[2]){
  int cur_heat_time = millis();
    digitalWrite(HEAT_PIN,HIGH);
    if((cur_heat_time - heat_start_time) < period[0]){
      digitalWrite(HEAT_PIN, HIGH);
    }
    else if((cur_heat_time - heat_start_time) < (period[0] + period[1])){
      digitalWrite(HEAT_PIN, LOW);
    }
    else heat_start_time = cur_heat_time;
}

//if temperature is too high, stop heating
void HeatBreak(){
    if(digitalRead(TEMP_SENSOR_PIN) > Safe_Temp){
      digitalWrite(HEAT_PIN, LOW);
    }
}


//------------------------------- Translate Signals ----------------------------------
void TransSW(){
  if(SW_state == "0000"){Inflate_SW = "00"; Heat_SW = "00";}
  else if(SW_state == "0100"){Inflate_SW = "01"; Heat_SW = "00";}
  else if(SW_state == "1000"){Inflate_SW = "10"; Heat_SW = "00";}
  else if(SW_state == "1100"){Inflate_SW = "11"; Heat_SW = "00";}
  else if(SW_state == "0001"){Inflate_SW = "00"; Heat_SW = "01";}
  else if(SW_state == "0101"){Inflate_SW = "01"; Heat_SW = "01";}
  else if(SW_state == "1001"){Inflate_SW = "10"; Heat_SW = "01";}
  else if(SW_state == "1101"){Inflate_SW = "11"; Heat_SW = "01";}
  else if(SW_state == "0011"){Inflate_SW = "00"; Heat_SW = "11";}
  else if(SW_state == "0111"){Inflate_SW = "01"; Heat_SW = "11";}
  else if(SW_state == "1011"){Inflate_SW = "10"; Heat_SW = "11";}
  else if(SW_state == "1111"){Inflate_SW = "11"; Heat_SW = "11";}
  else if(SW_state == "0010"){Inflate_SW = "00"; Heat_SW = "10";}
  else if(SW_state == "0110"){Inflate_SW = "01"; Heat_SW = "10";}
  else if(SW_state == "1010"){Inflate_SW = "10"; Heat_SW = "10";}
  else if(SW_state == "1110"){Inflate_SW = "11"; Heat_SW = "10";}
}

//------------------------------------ Main Setup -----------------------------------
void setup() {
    Serial.begin(115200);
    BLEClientSetup();
    pinMode(PUMP_PIN,OUTPUT);
    pinMode(VALVE_PIN_1,OUTPUT);
    pinMode(VALVE_PIN_2,OUTPUT);
    pinMode(HEAT_PIN,OUTPUT);
    pinMode(TEMP_SENSOR_PIN,INPUT);
    inflation_start_time = millis();
    heat_start_time = millis();
} 


//------------------------------------ Main Loop ------------------------------------
void loop() {
    int cur_inflation_time = millis();
    int cur_heat_time = millis();
    clientReceive();

    TransSW();

    //inflation
    if(Inflate_SW == "10")  InflationCycle(Inflation_Period1);
    else if(Inflate_SW == "01")  InflationCycle(Inflation_Period2);
    else if(Inflate_SW == "11")  InflationCycle(Inflation_Period3);
    else{
      digitalWrite(PUMP_PIN, LOW);
      digitalWrite(VALVE_PIN_1, LOW);
      digitalWrite(VALVE_PIN_2, LOW);
    }  

    //heating
    HeatBreak();
    if(Heat_SW == "10")  HeatCycle(Heat_Period1);
    else if(Heat_SW == "01")  HeatCycle(Heat_Period2);
    else if(Heat_SW == "11")  HeatCycle(Heat_Period3);
    else digitalWrite(HEAT_PIN, LOW);

}
