const int S1 = 18;
const int S2 = 17;
const int KEY = 16;

int lastState;

void setup() {

  Serial.begin(115200);

  pinMode(S1, INPUT_PULLUP);
  pinMode(S2, INPUT_PULLUP);
  pinMode(KEY, INPUT_PULLUP);

  int s1 = digitalRead(S1);
  int s2 = digitalRead(S2);

  lastState = (s1 << 1) | s2;

  Serial.println("EC11 Direction Test");
  Serial.println("-------------------");
}

void loop() {

  int s1 = digitalRead(S1);
  int s2 = digitalRead(S2);

  int currentState = (s1 << 1) | s2;

  if (currentState != lastState) {

    Serial.print("State: ");
    Serial.print(lastState);
    Serial.print(" -> ");
    Serial.print(currentState);

    if (
      (lastState == 0 && currentState == 2) ||
      (lastState == 2 && currentState == 3) ||
      (lastState == 3 && currentState == 1) ||
      (lastState == 1 && currentState == 0)
    ) {
      Serial.println("  CLOCKWISE");
    }

    else if (
      (lastState == 0 && currentState == 1) ||
      (lastState == 1 && currentState == 3) ||
      (lastState == 3 && currentState == 2) ||
      (lastState == 2 && currentState == 0)
    ) {
      Serial.println("  COUNTER-CLOCKWISE");
    }

    else {
      Serial.println("  INVALID");
    }

    lastState = currentState;
  }

  // Check push button separately
  if (digitalRead(KEY) == LOW) {
    Serial.println("KEY PRESSED");
    delay(200);
  }
}