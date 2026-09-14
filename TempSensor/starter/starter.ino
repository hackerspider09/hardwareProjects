#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);

  sensors.begin();

  Serial.println("DS18B20 Temperature Sensor");
  Serial.println("-------------------------");
}

void loop() {

  sensors.requestTemperatures();

  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);

  if (temperatureC == DEVICE_DISCONNECTED_C) {
    Serial.println("ERROR: DS18B20 not detected!");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperatureC);
    Serial.print(" °C");

    Serial.print("  |  ");

    Serial.print(temperatureF);
    Serial.println(" °F");
  }

  delay(1000);
}