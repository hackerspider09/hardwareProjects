#include <HijelHID_BLEMouse.h>
#include <Bounce2.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// =====================================================
// OLED
// =====================================================

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


// =====================================================
// Joystick / Button pins
// =====================================================

const int joyUp    = 4;
const int joyDown  = 5;
const int joyLeft  = 6;
const int joyRight = 7;

const int leftClick  = 8;
const int rightClick = 9;

const int autoMoveCurser = 3;


// =====================================================
// Auto cursor movement
// =====================================================

bool isAutoMoveCurserActive = false;

unsigned long lastAutoMoveTime = 0;

bool autoMoveDirection = false;


// =====================================================
// Mouse settings
// =====================================================

int range = 5;
int responseDelay = 10;


// =====================================================
// BLE Mouse
// =====================================================

HijelBLEMouse mouse("AirMouse");


// =====================================================
// Button debouncer
// =====================================================

Bounce debouncer = Bounce();


// =====================================================
// OLED setup
// =====================================================

void setup_OLED_display() {

  // Start I2C
  Wire.begin(OLED_SDA, OLED_SCL);

  // Start OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {

    Serial.println("OLED not found!");

    while (true) {
      delay(100);
    }
  }

  // Initial display setup
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("AirMouse");

  display.setCursor(0, 15);
  display.println("Starting...");

  display.display();
}


// =====================================================
// Display text on a specific OLED line
// =====================================================

String oledLines[4] = {"", "", "", ""};

void handlerOledText(int line, String text) {

  if (oledLines[line] == text) {
    return;
  }

  oledLines[line] = text;

  int y = line * 15;

  display.fillRect(0, y, SCREEN_WIDTH, 12, SSD1306_BLACK);

  display.setCursor(0, y);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.println(text);

  display.display();
}


// =====================================================
// Setup
// =====================================================

void setup() {

  Serial.begin(115200);

  // OLED
  setup_OLED_display();


  // ===================================================
  // Joystick switches
  // ===================================================

  pinMode(joyUp, INPUT_PULLUP);
  pinMode(joyDown, INPUT_PULLUP);
  pinMode(joyLeft, INPUT_PULLUP);
  pinMode(joyRight, INPUT_PULLUP);


  // ===================================================
  // Mouse buttons
  // ===================================================

  pinMode(leftClick, INPUT_PULLUP);
  pinMode(rightClick, INPUT_PULLUP);


  // ===================================================
  // Auto cursor button
  // ===================================================

  pinMode(autoMoveCurser, INPUT_PULLUP);

  // Attach Bounce2
  debouncer.attach(autoMoveCurser);

  // 5 ms debounce time
  debouncer.interval(5);


  // ===================================================
  // Start BLE mouse
  // ===================================================

  mouse.setBatteryLevel(100);
  mouse.begin();


  // ===================================================
  // Serial messages
  // ===================================================

  Serial.println("Air Mouse started");
  Serial.println("Waiting for Bluetooth connection...");


  // OLED
  handlerOledText(0, "AirMouse");
  handlerOledText(1, "Bluetooth: Waiting...");
  handlerOledText(2, "Mode: Normal");
}


// =====================================================
// Main loop
// =====================================================

void loop() {

  // ===================================================
  // Update button debounce
  // ===================================================

  debouncer.update();


  // ===================================================
  // Check Bluetooth connection
  // ===================================================

  if (!mouse.isPaired()) {

    handlerOledText(1, "Bluetooth: Waiting...");

    delay(100);

    return;
  }


  // Bluetooth connected
  handlerOledText(1, "Bluetooth: Connected");


  // ===================================================
  // Auto cursor button
  // ===================================================

  if (debouncer.fell()) {

    // Toggle auto mode
    isAutoMoveCurserActive =
      !isAutoMoveCurserActive;


    // Reset movement timer
    lastAutoMoveTime = millis();

    // Reset direction
    autoMoveDirection = false;


    // Serial output
    Serial.print("Auto move: ");

    Serial.println(
      isAutoMoveCurserActive
      ? "ON"
      : "OFF"
    );
  }


  // ===================================================
  // AUTO CURSOR MODE
  // ===================================================

  if (isAutoMoveCurserActive) {

    handlerOledText(2, "Mode: Auto");

    handleAutoMoveCurser();

    return;
  }


  // ===================================================
  // NORMAL MOUSE MODE
  // ===================================================

  handlerOledText(2, "Mode: Normal");


  // ===================================================
  // Read joystick
  // ===================================================

  int upState =
    digitalRead(joyUp);

  int downState =
    digitalRead(joyDown);

  int leftState =
    digitalRead(joyLeft);

  int rightState =
    digitalRead(joyRight);


  // ===================================================
  // Read mouse buttons
  // ===================================================

  int leftClickState =
    digitalRead(leftClick);

  int rightClickState =
    digitalRead(rightClick);


  // ===================================================
  // Handle mouse buttons
  // ===================================================

  handleMouseClick(
    leftClickState,
    rightClickState
  );


  // ===================================================
  // Calculate mouse movement
  // ===================================================

  int xDistance =
    (leftState - rightState) * range;

  int yDistance =
    (upState - downState) * range;


  // ===================================================
  // Move mouse
  // ===================================================

  if (xDistance != 0 || yDistance != 0) {

    mouse.move(
      xDistance,
      yDistance
    );
  }


  // Small response delay
  delay(responseDelay);
}


// =====================================================
// Mouse click handler
// =====================================================

void handleMouseClick(
  int leftClickState,
  int rightClickState
) {

  // ---------------------------------------------------
  // Left mouse button
  // ---------------------------------------------------

  if (leftClickState == LOW) {

    mouse.setButton(
      MouseButton::Left,
      true
    );

  } else {

    mouse.setButton(
      MouseButton::Left,
      false
    );
  }


  // ---------------------------------------------------
  // Right mouse button
  // ---------------------------------------------------

  if (rightClickState == LOW) {

    mouse.setButton(
      MouseButton::Right,
      true
    );

  } else {

    mouse.setButton(
      MouseButton::Right,
      false
    );
  }
}


// =====================================================
// Automatic cursor movement
// =====================================================

void handleAutoMoveCurser() {

  unsigned long currentTime =
    millis();


  // Check whether 500 ms has passed
  if (
    currentTime - lastAutoMoveTime >= 500
  ) {

    lastAutoMoveTime =
      currentTime;


    // -------------------------------------------------
    // Move UP
    // -------------------------------------------------

    if (autoMoveDirection == false) {

      mouse.move(0, -50);

      autoMoveDirection = true;
    }


    // -------------------------------------------------
    // Move DOWN
    // -------------------------------------------------

    else {

      mouse.move(0, 50);

      autoMoveDirection = false;
    }
  }
}