/*
 * BlE ESP32
 * For ESP32 on the remote controller 
 */

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SW_PIN_1 23 // switch 1
#define SW_PIN_2 22 // switch 2

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


void setup() {
    Serial.begin(115200);
    BLEServerSetup();
    pinMode(SW_PIN_1, INPUT);
    pinMode(SW_PIN_2, INPUT);
}

void loop() {
     if((digitalRead(SW_PIN_1) == HIGH)&(digitalRead(SW_PIN_2) == HIGH)) SW_state = "11";
     else if((digitalRead(SW_PIN_1) == LOW)&(digitalRead(SW_PIN_2) == HIGH)) SW_state = "01";
     else if((digitalRead(SW_PIN_1) == HIGH)&(digitalRead(SW_PIN_2) == LOW)) SW_state = "10";
     else SW_state = "00";
     pCharacteristic->setValue(SW_state);
     Serial.println(SW_state.c_str());
     //state = pCharacteristic->getValue().c_str();
     //num = atoi(pCharacteristic->getValue().c_str());
    //state = pCharacteristic->getValue().c_str();
    //Serial.println(state.c_str());
  // if(state == "00") Serial.println("yes0_0");
    delay(10);
}
