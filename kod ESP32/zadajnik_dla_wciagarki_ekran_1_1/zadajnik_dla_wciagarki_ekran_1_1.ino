#include "zmienne_globalne.h"
#include "wejscia_wyjscia.h"
#include "ekran.h"
#include "komunikacja_modbusrtu.h"
#include "obsluga_wifi.h"
#include "obsluga_web_serwera.h"



void setup() {
  init_on_reset();
  init_przyciski();
    init_oled_screen();
  targetPosition = wczytaj_preset_z_pamieci(ktory_preset_wybrany);
  Serial.begin(115200);
  init_modbus_rtu();
  init_wifi();
  init_all_web_pages();

}

void loop() {
  
  
}
