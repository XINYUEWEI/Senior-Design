/*
 * SENIOR DESIGN PROJECT
 * TEAM: CRAMPR DAMPR
 * BlE ESP32
 * For ESP32 on the remote controller 
 */

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define IN_SW_1 23 // Inflation switch pin 1
#define IN_SW_2 22 // Inflation switch pin 2
#define H_SW_1 23 // Heating switch pin 1
#define H_SW_2 22 // Heating switch pin 2


//------------------------------------ Bluetooth --------------------------------------
#define SERVICE_UUID    "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID   "beb5483e-36e1-4688-b7f5-ea07361b26a8"

static BLEServer *pServer;
static BLEService *pService;
static BLECharacteristic *pCharacteristic;
static BLEAdvertising *pAdvertising;

std::string SW_state = "00";
std::string state = "11";

void BLEServerSetup(){
    BLEDevice::init("Long name works now");
    pServer = BLEDevice::createServer();
    pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_READ |
                        BLECharacteristic::PROPERTY_WRITE
                      );
    pCharacteristic->setValue("Hello! BLE starts!");
    pService->start();
    pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    //Serial.println("Characteristic defined!");
}

//------------------------------------ Encode Switch Signals --------------------------------------
void EncodeSW(){
  if((digitalRead(IN_SW_1)==HIGH)&(digitalRead(IN_SW_2)==HIGH)&(digitalRead(H_SW_1)==HIGH)&(digitalRead(H_SW_2)==HIGH)) SW_state = "1111";
  else if((digitalRead(IN_SW_1)==LOW)&(digitalRead(IN_SW_2)==HIGH)&(digitalRead(H_SW_1)==HIGH)&(digitalRead(H_SW_2)==HIGH)) SW_state = "0111";
  else if((digitalRead(IN_SW_1)==LOW)&(digitalRead(IN_SW_2)==LOW)&(digitalRead(H_SW_1)==HIGH)&(digitalRead(H_SW_2)==HIGH)) SW_state = "0011";
  else if((digitalRead(IN_SW_1)==HIGH)&(digitalRead(IN_SW_2)==LOW)&(digitalRead(H_SW_1)==HIGH)&(digitalRead(H_SW_2)==HIGH)) SW_state = "1011";
  
  else if((digitalRead(IN_SW_1)==HIGH)&(digitalRead(IN_SW_2)==HIGH)&(digitalRead(H_SW_1)==LOW)&(digitalRead(H_SW_2)==HIGH)) SW_state = "1101";
  else if((digitalRead(IN_SW_1)==LOW)&(digitalRead(IN_SW_2)==HIGH)&(digitalRead(H_SW_1)==LOW)&(digitalRead(H_SW_2)==HIGH)) SW_state = "0101";
  else if((digitalRead(IN_SW_1)==LOW)&(digitalRead(IN_SW_2)==LOW)&(digitalRead(H_SW_1)==LOW)&(digitalRead(H_SW_2)==HIGH)) SW_state = "0001";
  else if((digitalRead(IN_SW_1)==HIGH)&(digitalRead(IN_SW_2)==LOW)&(digitalRead(H_SW_1)==LOW)&(digitalRead(H_SW_2)==HIGH)) SW_state = "1001";
  
  else if((digitalRead(IN_SW_1)==HIGH)&(digitalRead(IN_SW_2)==HIGH)&(digitalRead(H_SW_1)==LOW)&(digitalRead(H_SW_2)==LOW)) SW_state = "1100";
  else if((digitalRead(IN_SW_1)==LOW)&(digitalRead(IN_SW_2)==HIGH)&(digitalRead(H_SW_1)==LOW)&(digitalRead(H_SW_2)==LOW)) SW_state = "0100";
  else if((digitalRead(IN_SW_1)==LOW)&(digitalRead(IN_SW_2)==LOW)&(digitalRead(H_SW_1)==LOW)&(digitalRead(H_SW_2)==LOW)) SW_state = "0000";
  else if((digitalRead(IN_SW_1)==HIGH)&(digitalRead(IN_SW_2)==LOW)&(digitalRead(H_SW_1)==LOW)&(digitalRead(H_SW_2)==LOW)) SW_state = "1000";

  else if((digitalRead(IN_SW_1)==HIGH)&(digitalRead(IN_SW_2)==HIGH)&(digitalRead(H_SW_1)==HIGH)&(digitalRead(H_SW_2)==LOW)) SW_state = "1110";
  else if((digitalRead(IN_SW_1)==LOW)&(digitalRead(IN_SW_2)==HIGH)&(digitalRead(H_SW_1)==HIGH)&(digitalRead(H_SW_2)==LOW)) SW_state = "0110";
  else if((digitalRead(IN_SW_1)==LOW)&(digitalRead(IN_SW_2)==LOW)&(digitalRead(H_SW_1)==HIGH)&(digitalRead(H_SW_2)==LOW)) SW_state = "0010";
  else if((digitalRead(IN_SW_1)==HIGH)&(digitalRead(IN_SW_2)==LOW)&(digitalRead(H_SW_1)==HIGH)&(digitalRead(H_SW_2)==LOW)) SW_state = "1010";
}

//------------------------------------ Main Setup and Main Loop --------------------------------------
void setup() {
    Serial.begin(115200);
    BLEServerSetup();
    pinMode(IN_SW_1, INPUT);
    pinMode(IN_SW_2, INPUT);
    pinMode(H_SW_1, INPUT);
    pinMode(H_SW_2, INPUT);
}

void loop() {
    EncodeSW();
    pCharacteristic->setValue(SW_state);
    Serial.println(SW_state.c_str());
    delay(10);
}
