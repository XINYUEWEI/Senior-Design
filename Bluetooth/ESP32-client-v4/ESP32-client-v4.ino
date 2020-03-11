/*
 * BlE ESP32
 * For ESP32 on the belt
 */


#include "BLEDevice.h"

#define   PUMP_PIN          22 
#define   VALVE_PIN         23
#define   HEAT_PIN          32
#define   TEMP_SENSOR_PIN   33


int Period1[2] = {3000,2000}; //infalte | deflate (unit:ms)
int Period2[2] = {4000,2000};
int Period3[2] = {5000,2000};

std::string SW_state = "00";
int start_time = 0;

//------------------------------ Bluetooth -------------------------------
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


//------------------------------ Inflation -------------------------------
void InflationCycle(int period[2]){
    int cur_time = millis();
    if((cur_time - start_time) < period[0]){
      digitalWrite(PUMP_PIN, HIGH);
      digitalWrite(VALVE_PIN, LOW);
    }
    else if((cur_time - start_time) < (period[0] + period[1])){
      digitalWrite(PUMP_PIN, LOW);
      digitalWrite(VALVE_PIN, HIGH);
    }
    else start_time = cur_time;
}


//------------------------------ Heating ---------------------------------
void Heating(){
  
}


void setup() {
    Serial.begin(115200);
    BLEClientSetup();
    pinMode(PUMP_PIN,OUTPUT);
    pinMode(VALVE_PIN,OUTPUT);
    pinMode(HEAT_PIN,OUTPUT);
    pinMode(TEMP_SENSOR_PIN,INPUT);
    start_time = millis();
} 

void loop() {
    int cur_time = millis();
    clientReceive();

    if(SW_state == "10")  InflationCycle(Period1);
    else if(SW_state == "01")  InflationCycle(Period2);
    else if(SW_state == "11")  InflationCycle(Period3);
    else{
      digitalWrite(PUMP_PIN, LOW);
      digitalWrite(VALVE_PIN, LOW);
    }  
}
