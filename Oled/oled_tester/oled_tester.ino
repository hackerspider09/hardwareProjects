#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA 11
#define OLED_SCL 10

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

void setup() {

  Serial.begin(115200);

  // Start I2C
  Wire.begin(OLED_SDA, OLED_SCL);

  // Start OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
    while (true);
  }

  Serial.println("OLED started!");

  // Clear screen
  display.clearDisplay();

  // Text settings
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Position
  display.setCursor(0, 0);

  // Text
  display.println("Air Mouse");

  display.setCursor(0, 15);
  display.println("ESP32-S3");

  display.setCursor(0, 30);
  display.println("Bluetooth: OK");

  display.setCursor(0, 45);
  display.println("Ready!");

  // Actually send buffer to OLED
  display.display();
}

void loop() {
}