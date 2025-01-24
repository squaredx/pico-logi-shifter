#include <Joystick.h>

#define X_AXIS_PIN 26
#define Y_AXIS_PIN 28

#define REVERSE_PIN 9

#define X_LEFT_THRESHOLD 300
#define X_RIGHT_THRESHOLD 650

#define Y_TOP_THRESHOLD 650
#define Y_BOTTOM_THRESHOLD 250

enum Gear {
  ONE,
  TWO,
  THREE,
  FOUR,
  FIVE,
  SIX,
  REVERSE,
  NEUTRAL
};

Gear currentGear = Gear::NEUTRAL;

void setup() {
  pinMode(X_AXIS_PIN, INPUT_PULLUP);
  pinMode(Y_AXIS_PIN, INPUT_PULLUP);
  pinMode(REVERSE_PIN, INPUT_PULLUP);

  Joystick.begin();

  Serial.begin(115200);
  while (!Serial && millis() < 10000UL);
  Serial.println("Shifter Ready");
}

void loop() {
  int x = analogRead(X_AXIS_PIN);
  int y = analogRead(Y_AXIS_PIN);
  int reverse = digitalRead(REVERSE_PIN);

  Gear newGear = determineGear(x, y, reverse);

  if(currentGear != newGear) {
    currentGear = newGear;
    updateJoystick();
    Serial.println("Gear changed to: " + gearToString(currentGear));
  }

  delay(20);
}

Gear determineGear(int x, int y, bool reverse) {
  if (y > Y_TOP_THRESHOLD) {
    if (x < X_LEFT_THRESHOLD) return Gear::ONE;
    if (x < X_RIGHT_THRESHOLD) return Gear::THREE;
    return Gear::FIVE;
  }

  if (y < Y_BOTTOM_THRESHOLD) {
    if (x < X_LEFT_THRESHOLD) return Gear::TWO;
    if (x < X_RIGHT_THRESHOLD) return Gear::FOUR;
    return reverse ? Gear::REVERSE : Gear::SIX;
  }

  return Gear::NEUTRAL;
}

void updateJoystick() {
  for(uint8_t i = 0; i < 8; i++) {
    Joystick.button(i+1, i == currentGear);
  }
}

String gearToString(Gear gear) {
  switch (gear) {
    case Gear::ONE: return "ONE";
    case Gear::TWO: return "TWO";
    case Gear::THREE: return "THREE";
    case Gear::FOUR: return "FOUR";
    case Gear::FIVE: return "FIVE";
    case Gear::SIX: return "SIX";
    case Gear::REVERSE: return "REVERSE";
    case Gear::NEUTRAL: return "NEUTRAL";
    default: return "UNKNOWN";
  }
}
