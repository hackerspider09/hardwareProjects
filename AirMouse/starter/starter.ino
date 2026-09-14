#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

const int ledPin = 12; // Onboard LED pin
bool deviceConnected = false;

// Custom callback class to handle connection events
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("Device connected!");
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println("Device disconnected!");
      // Restart advertising so other devices can find it again
      pServer->startAdvertising(); 
    }
};

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // Initialize BLE Device
  BLEDevice::init("ESP32_BLE_Blinker");

  // Create BLE Server and set its callbacks
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Start BLE Advertising so your phone can see it
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->start();
  
  Serial.println("BLE waiting for a client connection...");
}

void loop() {
  if (deviceConnected) {
    // Blink LED when connected
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
  } else {
    // Turn off LED when disconnected
    digitalWrite(ledPin, LOW);
  }
}
