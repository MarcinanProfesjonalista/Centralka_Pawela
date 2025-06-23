#include "zmienne_globalne.h"
#include "wejscia_wyjscia.h"
#include "ekran.h"
#include "komunikacja_modbusrtu.h"
//#include "komunikacja_modbusrtu.h"


void setup() {
  init_on_reset();
  init_oled_screen();
  targetPosition = wczytaj_preset_z_pamieci(ktory_preset_wybrany);
  Serial.begin(115200);
  init_modbus_rtu();
}

void loop() {
  obsluz_przyciski();
  obsluga_wyswietlania();
}
