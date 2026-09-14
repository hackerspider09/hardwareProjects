const int joyUp    = 4;
const int joyDown  = 5;
const int joyLeft  = 6;
const int joyRight = 7;
const int joyMid   = 8;

void setup() {
  Serial.begin(115200);

  pinMode(joyUp,    INPUT_PULLUP);
  pinMode(joyDown,  INPUT_PULLUP);
  pinMode(joyLeft,  INPUT_PULLUP);
  pinMode(joyRight, INPUT_PULLUP);
  pinMode(joyMid,   INPUT_PULLUP);

  Serial.println("5D Joystick Test");
}

void loop() {

  Serial.print("UP: ");
  Serial.print(digitalRead(joyUp));

  Serial.print(" | DOWN: ");
  Serial.print(digitalRead(joyDown));

  Serial.print(" | LEFT: ");
  Serial.print(digitalRead(joyLeft));

  Serial.print(" | RIGHT: ");
  Serial.print(digitalRead(joyRight));

  Serial.print(" | MID: ");
  Serial.println(digitalRead(joyMid));

  delay(200);
}