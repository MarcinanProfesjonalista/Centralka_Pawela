#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

TaskHandle_t wyswietlanieTaskHandle = NULL;

///Zmienne srodowiskowe
int widok_wyswietlany = 0;  // 0 --> status, 1 --> presety,
int menu_index = 0;         //zaczynamy od status 1

int ktory_preset_wybrany = 0;

const unsigned long debounceDelay = 200;

// Zmienne do śledzenia czasu ostatnich naciśnięć przycisków
unsigned long lastDebounceTimeAccept = 0;
unsigned long lastDebounceTimeBack = 0;
unsigned long lastDebounceTimeUp = 0;
unsigned long lastDebounceTimeDown = 0;


// Do UP/DOWN
unsigned long buttonUpStartTime = 0;
unsigned long buttonDownStartTime = 0;
unsigned long lastRepeatTime = 0;


//const unsigned long debounceDelay = 50; // Czas na debouncing
const unsigned long holdInterval = 300;                       // Interwał zmiany wartości przy dłuższym przytrzymaniu
const unsigned long ile_czekac_przed_duzymi_zmianami = 2001;  // Interwał zmiany wartości przy dłuższym przytrzymaniu


#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



void wyswietl_adres_i_status(byte id = 0, byte ktory_preset_wybrany = 0, float aktualna_pozycja = 0, float targetPosition = 0, bool krGorna = false, bool krDolna = false, bool czy_ma_jechac = false) {

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Wyswietlanie ID i kierunku jazdy
  display.setCursor(0, 0);
  display.print("ID: ");
  display.print(id);

  if (czy_mam_komunikacje) {
    display.print(" KOM OK");
  } else {
    display.print(" BRAK KOM");
  }
  display.setCursor(0, 10);
  display.print("Preset: ");
  display.print(ktory_preset_wybrany);

  display.setCursor(110, 0);
  display.print(krGorna ? "--" : "/\\");

  display.setCursor(110, 10);
  display.print(krDolna ? "__" : "\\/");

  // Informacja o celu
  display.setCursor(0, 20);
  display.print("Akt:");
  display.print(aktualna_pozycja, 2);
  display.setCursor(0, 30);
  display.print("Cel:");
  display.print(targetPosition, 2);  // 2 miejsca po przecinku
  display.setCursor(0, 40);
  if (czy_ma_jechac) {
    display.print("Teraz jedzie");
  } else {
    display.print("Teraz stoi");
  }

  display.setCursor(0, 54);
  display.print("+   -   presety");
  if (czy_ma_jechac) {
    display.setCursor(104, 54);
    display.print("stoj");
  } else {
    display.setCursor(104, 54);
    display.print("jedz");
  }

  display.display();
}

void handleStatusActions() {
  unsigned long currentMillis = millis();

  // === ACCEPT ===
  if (BUTTON_ACCEPT_PIN_wcisniety) {
    if ((currentMillis - lastDebounceTimeAccept) > debounceDelay) {
      if (BUTTON_ACCEPT_PIN_pamiec == true) {
        BUTTON_ACCEPT_PIN_pamiec = false;
        if (czy_ma_jechac == true) {
          czy_ma_jechac = false;
        } else {
          czy_ma_jechac = true;
        }
      }
    }
    lastDebounceTimeAccept = currentMillis;
  } else {
    BUTTON_ACCEPT_PIN_pamiec = true;
  }

  // === BACK ===
  if (BUTTON_BACK_PIN_wcisniety) {
    if ((currentMillis - lastDebounceTimeBack) > debounceDelay) {
      if (BUTTON_BACK_PIN_pamiec == true) {
        widok_wyswietlany = 1;

        BUTTON_BACK_PIN_pamiec = false;
      }
    }
    lastDebounceTimeBack = currentMillis;
  } else {
    BUTTON_BACK_PIN_pamiec = true;
  }

  // === UP ===
  if (BUTTON_UP_PIN_wcisniety) {
    if (buttonUpStartTime == 0) buttonUpStartTime = currentMillis;

    if ((currentMillis - lastRepeatTime) > holdInterval) {
      if ((currentMillis - buttonUpStartTime) >= ile_czekac_przed_duzymi_zmianami) {
        targetPosition += 1.0;
      } else {
        targetPosition += 0.05;
      }
      lastRepeatTime = currentMillis;
    }
  } else {
    buttonUpStartTime = 0;
  }

  // === DOWN ===
  if (BUTTON_DOWN_PIN_wcisniety) {
    if (buttonDownStartTime == 0) buttonDownStartTime = currentMillis;

    if ((currentMillis - lastRepeatTime) > holdInterval) {
      if ((currentMillis - buttonDownStartTime) >= ile_czekac_przed_duzymi_zmianami) {
        targetPosition -= 1.0;
      } else {
        targetPosition -= 0.05;
      }
      lastRepeatTime = currentMillis;
    }
  } else {
    buttonDownStartTime = 0;
  }
}

void wyswietl_liste_presetow() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Wysokość pola listy: od 0 do 47 (6 linii po 8px)
  for (int i = 0; i < 5; i++) {
    display.setCursor(0, i * 10);  // interlinia co 10px

    if (i == ktory_preset_wybrany) {
      display.print(">");
    } else {
      display.print(" ");
    }
    display.print("[");
    display.print(i);
    display.print("]:");
    display.print(preset[i]);
  }

  // Pasek sterowania na dole
  display.setCursor(0, 54);
  display.print("UP DOWN  edytuj");

  display.setCursor(104, 54);
  if (czy_ma_jechac) {
    display.print("stoj");
  } else {
    display.print("jedz");
  }

  display.display();
}

void handlePresetListActions() {
  unsigned long currentMillis = millis();
  // === ACCEPT ===
  if (BUTTON_ACCEPT_PIN_wcisniety) {
    if ((currentMillis - lastDebounceTimeAccept) > debounceDelay) {
      if (BUTTON_ACCEPT_PIN_pamiec) {
        BUTTON_ACCEPT_PIN_pamiec = false;
        czy_ma_jechac = !czy_ma_jechac;
      }
      lastDebounceTimeAccept = currentMillis;
    }
  } else {
    BUTTON_ACCEPT_PIN_pamiec = true;
  }

  // === BACK (wejdź do edycji) ===
  if (BUTTON_BACK_PIN_wcisniety) {
    if ((currentMillis - lastDebounceTimeBack) > debounceDelay) {
      if (BUTTON_BACK_PIN_pamiec) {
        widok_wyswietlany = 0;
        BUTTON_BACK_PIN_pamiec = false;
        // resetuj po zmianie widoku
      }
      lastDebounceTimeBack = currentMillis;
    }
  } else {
    BUTTON_BACK_PIN_pamiec = true;
  }

  // === UP ===
  if (BUTTON_UP_PIN_wcisniety) {
    if ((currentMillis - lastDebounceTimeUp) > debounceDelay) {
      if (BUTTON_UP_PIN_pamiec) {
        BUTTON_UP_PIN_pamiec = false;
        ktory_preset_wybrany--;

        if (ktory_preset_wybrany < 0) ktory_preset_wybrany = 0;
        czy_ma_jechac = false;
        targetPosition = preset[ktory_preset_wybrany];
      }
      lastDebounceTimeUp = currentMillis;
    }
  } else {
    BUTTON_UP_PIN_pamiec = true;
  }

  // === DOWN ===
  if (BUTTON_DOWN_PIN_wcisniety) {
    if ((currentMillis - lastDebounceTimeDown) > debounceDelay) {
      if (BUTTON_DOWN_PIN_pamiec) {
        BUTTON_DOWN_PIN_pamiec = false;
        ktory_preset_wybrany++;
        if (ktory_preset_wybrany > 4) ktory_preset_wybrany = 4;
        czy_ma_jechac = false;
        targetPosition = preset[ktory_preset_wybrany];
      }
      lastDebounceTimeDown = currentMillis;
    }
  } else {
    BUTTON_DOWN_PIN_pamiec = true;
  }
}

void wyswietl_czy_zapisac_preset() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Wyswietlanie ID i kierunku jazdy
  display.setCursor(0, 0);
  display.print("Czy zapisac preset? ");
  display.setCursor(0, 10);
  display.print("[");
  display.print(ktory_preset_wybrany);
  display.print("]:");
  display.print(targetPosition);

  display.setCursor(0, 54);
  display.print("edytuj tak  nie");
  if (czy_ma_jechac) {
    display.setCursor(104, 54);
    display.print("stoj");
  } else {
    display.setCursor(104, 54);
    display.print("jedz");
  }
  display.display();
}

void handleSavePresetActions() {
  unsigned long currentMillis = millis();

  // === jedz/niejedz ===
  if (BUTTON_ACCEPT_PIN_wcisniety) {
    if ((currentMillis - lastDebounceTimeAccept) > debounceDelay) {
      if (BUTTON_ACCEPT_PIN_pamiec) {
        BUTTON_ACCEPT_PIN_pamiec = false;
        czy_ma_jechac = !czy_ma_jechac;
      }
      lastDebounceTimeAccept = currentMillis;
    }
  } else {
    BUTTON_ACCEPT_PIN_pamiec = true;
  }

  // === nie_zapisuj ===
  if (BUTTON_BACK_PIN_wcisniety) {
    if ((currentMillis - lastDebounceTimeBack) > debounceDelay) {
      if (BUTTON_BACK_PIN_pamiec) {
        targetPosition = wczytaj_preset_z_pamieci(ktory_preset_wybrany);
        wczytaj_presety();
        czy_ma_jechac = false;

        widok_wyswietlany = 2;
        BUTTON_BACK_PIN_pamiec = false;
      }
      lastDebounceTimeBack = currentMillis;
    }
  } else {
    BUTTON_BACK_PIN_pamiec = true;
  }

  // === UP – anuluj zapis, wróć do edycji ===
  if (BUTTON_UP_PIN_wcisniety) {
    if ((currentMillis - lastDebounceTimeUp) > debounceDelay) {
      if (BUTTON_UP_PIN_pamiec) {
        BUTTON_UP_PIN_pamiec = false;
        //serial.println("Anulowano zapis – powrót do edycji");
        widok_wyswietlany = 0;
      }
      lastDebounceTimeUp = currentMillis;
    }
  } else {
    BUTTON_UP_PIN_pamiec = true;
  }

  // === DOWN – zapisz preset, wróć do listy ===
  if (BUTTON_DOWN_PIN_wcisniety) {
    if ((currentMillis - lastDebounceTimeDown) > debounceDelay) {
      if (BUTTON_DOWN_PIN_pamiec) {
        BUTTON_DOWN_PIN_pamiec = false;
        //serial.println("Zapisano preset");
        zapisz_preset_do_pamieci(ktory_preset_wybrany, targetPosition);
        wczytaj_presety();
        widok_wyswietlany = 2;
      }
      lastDebounceTimeDown = currentMillis;
    }
  } else {
    BUTTON_DOWN_PIN_pamiec = true;
  }
}

void obsluga_wyswietlania() {
  switch (widok_wyswietlany) {
    case 0:
      wyswietl_adres_i_status(id, ktory_preset_wybrany, aktualna_pozycja, targetPosition, krGorna, krDolna, czy_ma_jechac);
      handleStatusActions();
      break;
    case 1:
      wyswietl_czy_zapisac_preset();
      handleSavePresetActions();
      break;
    case 2:
      wyswietl_liste_presetow();
      handlePresetListActions();
      break;
    default:
      wyswietl_adres_i_status(id, ktory_preset_wybrany, aktualna_pozycja, targetPosition, krGorna, krDolna, czy_ma_jechac);
      handleStatusActions();
      break;
  }
}

void wyswietlanieTask(void* parameter) {
  for (;;) {
    obsluga_wyswietlania();                // Twoja funkcja rysująca
    vTaskDelay(100 / portTICK_PERIOD_MS);  // Odświeżanie co 100 ms (możesz dostosować)
  }
}

void init_oled_screen() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  xTaskCreatePinnedToCore(
    wyswietlanieTask,         // Funkcja zadania
    "WyswietlanieTask",       // Nazwa zadania
    4096,                     // Stos (jeśli masz dużo rysowania, 4kB to minimum)
    NULL,                     // Parametr
    1,                        // Priorytet
    &wyswietlanieTaskHandle,  // Uchwyt
    1                         // Rdzeń (np. rdzeń 1)
  );
}
