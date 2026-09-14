#include <BLEDevice.h>
#include <BLEServer.h>

void setup() {
  Serial.begin(115200);
  delay(1000);

  BLEDevice::init("ESP32-AirMouse");

  BLEServer *server = BLEDevice::createServer();

  BLEAdvertising *advertising = BLEDevice::getAdvertising();
  advertising->start();

  Serial.println("BLE started!");
  Serial.println("Advertising as: ESP32-AirMouse");
}

void loop() {
  delay(1000);
}