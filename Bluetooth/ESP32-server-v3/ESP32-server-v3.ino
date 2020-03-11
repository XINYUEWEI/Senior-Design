//server

//version 3 is to try to convert the sent data from string to int

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
//#include <iostream>

#define SERVICE_UUID    "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID   "beb5483e-36e1-4688-b7f5-ea07361b26a8"

static BLEServer *pServer;
static BLEService *pService;
static BLECharacteristic *pCharacteristic;
static BLEAdvertising *pAdvertising;

int i = 0;
int state = 0;
std::string sta = "0";

void setup() {
    Serial.begin(115200);
    Serial.println("Starting BLE work!");

    BLEDevice::init("Long name works now");
    pServer = BLEDevice::createServer();
    pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_READ |
                        BLECharacteristic::PROPERTY_WRITE
                      );
    pCharacteristic->setValue("Hello");
    pService->start();
    pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    Serial.println("Characteristic defined!");
}

void loop() {
    // put your main code here, to run repeatedly:
     pCharacteristic->setValue("1");
     state = atoi(pCharacteristic->getValue().c_str());
     //Serial.println(num);
//    i++;
//    int sta = pCharacteristic->getValue();
    //state = pCharacteristic->getValue().c_str();
    if(state == 1) Serial.println("yes");
   // Serial.println(state);
    //Serial.println(pCharacteristic->getValue().c_str());
    delay(500);
}
