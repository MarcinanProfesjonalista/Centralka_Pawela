#include "esp32-hal-gpio.h"

TaskHandle_t przyciskiTaskHandle = NULL;

#define BUTTON_UP_PIN 14
#define BUTTON_DOWN_PIN 27
#define BUTTON_BACK_PIN 26
#define BUTTON_ACCEPT_PIN 25

#define WEJSCIE_IN1 36
#define WEJSCIE_IN2 39
#define WEJSCIE_IN3 34
#define WEJSCIE_IN4 35
#define WEJSCIE_IN5 32
#define WEJSCIE_IN6 33

bool BUTTON_UP_PIN_wcisniety = false;
bool BUTTON_DOWN_PIN_wcisniety = false;
bool BUTTON_BACK_PIN_wcisniety = false;
bool BUTTON_ACCEPT_PIN_wcisniety = false;

bool WEJSCIE_IN1_aktywne = false;
bool WEJSCIE_IN2_aktywne = false;
bool WEJSCIE_IN3_aktywne = false;
bool WEJSCIE_IN4_aktywne = false;
bool WEJSCIE_IN5_aktywne = false;
bool WEJSCIE_IN6_aktywne = false;


bool BUTTON_UP_PIN_pamiec = false;
bool BUTTON_DOWN_PIN_pamiec = false;
bool BUTTON_BACK_PIN_pamiec = false;
bool BUTTON_ACCEPT_PIN_pamiec = false;



void obsluz_przyciski() {
  pinMode(25, INPUT_PULLUP);
  BUTTON_UP_PIN_wcisniety     = (digitalRead(BUTTON_UP_PIN) == LOW);
  BUTTON_DOWN_PIN_wcisniety   = (digitalRead(BUTTON_DOWN_PIN) == LOW);
  BUTTON_BACK_PIN_wcisniety   = (digitalRead(BUTTON_BACK_PIN) == LOW);
  BUTTON_ACCEPT_PIN_wcisniety = (digitalRead(BUTTON_ACCEPT_PIN) == LOW);

  // if (BUTTON_UP_PIN_wcisniety)     Serial.println("UP");
  // if (BUTTON_DOWN_PIN_wcisniety)   Serial.println("DOWN");
  // Możesz dodać też print dla pozostałych, jeśli potrzebujesz debugowania

  // Obsługa wejść cyfrowych
  WEJSCIE_IN1_aktywne = (digitalRead(WEJSCIE_IN1) == LOW);
  WEJSCIE_IN2_aktywne = (digitalRead(WEJSCIE_IN2) == LOW);
  WEJSCIE_IN3_aktywne = (digitalRead(WEJSCIE_IN3) == LOW);
  WEJSCIE_IN4_aktywne = (digitalRead(WEJSCIE_IN4) == LOW);
  WEJSCIE_IN5_aktywne = (digitalRead(WEJSCIE_IN5) == LOW);
  WEJSCIE_IN6_aktywne = (digitalRead(WEJSCIE_IN6) == LOW);
}

void przyciskiTask(void* parameter) {
  for (;;) {
    obsluz_przyciski();        // Twoja funkcja
    vTaskDelay(30 / portTICK_PERIOD_MS);  // Odświeżanie co 30 ms
  }
}

void init_przyciski() {
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  pinMode(BUTTON_BACK_PIN, INPUT_PULLUP);
  pinMode(BUTTON_ACCEPT_PIN, INPUT_PULLUP);

  pinMode(WEJSCIE_IN1, INPUT);
  pinMode(WEJSCIE_IN2, INPUT);
  pinMode(WEJSCIE_IN3, INPUT);
  pinMode(WEJSCIE_IN4, INPUT);
  pinMode(WEJSCIE_IN5, INPUT);
  pinMode(WEJSCIE_IN6, INPUT);

    // Tworzenie wątku
  xTaskCreatePinnedToCore(
    przyciskiTask,       // Funkcja zadania
    "PrzyciskiTask",     // Nazwa
    2048,                // Stos
    NULL,                // Parametr
    1,                   // Priorytet
    &przyciskiTaskHandle,// Uchwyt
    1                    // Rdzeń (0 lub 1)
  );
}