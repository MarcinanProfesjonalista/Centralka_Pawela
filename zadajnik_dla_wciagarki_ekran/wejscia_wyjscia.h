
#define BUTTON_UP_PIN 15
#define BUTTON_DOWN_PIN 27
#define BUTTON_BACK_PIN 26
#define BUTTON_ACCEPT_PIN 25

bool BUTTON_UP_PIN_wcisniety = false;
bool BUTTON_DOWN_PIN_wcisniety = false;
bool BUTTON_BACK_PIN_wcisniety = false;
bool BUTTON_ACCEPT_PIN_wcisniety = false;


bool BUTTON_UP_PIN_pamiec = false;
bool BUTTON_DOWN_PIN_pamiec = false;
bool BUTTON_BACK_PIN_pamiec = false;
bool BUTTON_ACCEPT_PIN_pamiec = false;

void obsluz_przyciski() {
pinMode(25, INPUT_PULLUP);
  if (digitalRead(BUTTON_UP_PIN) == LOW) {
    BUTTON_UP_PIN_wcisniety = true;
  } else {
    BUTTON_UP_PIN_wcisniety = false;
  }
  if (digitalRead(BUTTON_DOWN_PIN) == LOW) {
    // Serial.println("down");
    BUTTON_DOWN_PIN_wcisniety = true;
  } else {
    BUTTON_DOWN_PIN_wcisniety = false;
  }
  if (digitalRead(BUTTON_BACK_PIN) == LOW) {
    BUTTON_BACK_PIN_wcisniety = true;
  } else {
    BUTTON_BACK_PIN_wcisniety = false;
  }
  if (digitalRead(BUTTON_ACCEPT_PIN) == LOW) {
    //Serial.println("Accept");
    BUTTON_ACCEPT_PIN_wcisniety = true;
  } else {
    BUTTON_ACCEPT_PIN_wcisniety = false;
  }
}