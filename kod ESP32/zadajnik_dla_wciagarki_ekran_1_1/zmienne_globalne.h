#include "przydatne_funkcje.h"

int id = 1;
bool czy_mam_komunikacje = false;
float targetPosition = 0;  //w metrach
bool czy_ma_jechac = false;
float aktualna_pozycja = 0;  //w metrach

bool krGorna = false;
bool krDolna = false;

float preset[5] = { 1.23, 4.56, 7.89, 10.11, -11 };


void zapisz_preset_do_pamieci(int ktory_preset_wybrany = 0, float targetPosition = 0) {
  Serial.print("Zapisuje:[");
  Serial.print(ktory_preset_wybrany);
  Serial.print("]:");
  Serial.println(targetPosition);


  if (ktory_preset_wybrany == 0) {
    EEPROMWriteFloat(EEPROM_PRESET1_ADDRES, targetPosition);
  }
  if (ktory_preset_wybrany == 1) {
    EEPROMWriteFloat(EEPROM_PRESET2_ADDRES, targetPosition);
  }
  if (ktory_preset_wybrany == 2) {
    EEPROMWriteFloat(EEPROM_PRESET3_ADDRES, targetPosition);
  }
  if (ktory_preset_wybrany == 3) {
    EEPROMWriteFloat(EEPROM_PRESET4_ADDRES, targetPosition);
  }
  if (ktory_preset_wybrany == 4) {
    EEPROMWriteFloat(EEPROM_PRESET5_ADDRES, targetPosition);
  }
}

float wczytaj_preset_z_pamieci(int ktory_preset_wybrany = 0) {
  float wynik = 0;
  Serial.print("wczytuje:[");
  Serial.print(ktory_preset_wybrany);
  Serial.print("]:");
  Serial.println(targetPosition);
  if (ktory_preset_wybrany == 0) {
    wynik = EEPROMReadFloat(EEPROM_PRESET1_ADDRES);
  }
  if (ktory_preset_wybrany == 1) {
    wynik = EEPROMReadFloat(EEPROM_PRESET2_ADDRES);
  }
  if (ktory_preset_wybrany == 2) {
    wynik = EEPROMReadFloat(EEPROM_PRESET3_ADDRES);
  }
  if (ktory_preset_wybrany == 3) {
    wynik = EEPROMReadFloat(EEPROM_PRESET4_ADDRES);
  }
  if (ktory_preset_wybrany == 4) {
    wynik = EEPROMReadFloat(EEPROM_PRESET5_ADDRES);
  }
  return wynik;
}

void EEPROMWipeOnce() {
  if (EEPROM.read(EEPROM_FLAG_ADDRESS) != 0xAA) {
    Serial.println("czyszczenie pamieci raz. Instalacja zmiennych");
    // Ustawienie domyślnych wartości
    //float preset[5]={1.23,4.56,7.89,10.11,-11};
    EEPROMWriteFloat(EEPROM_PRESET1_ADDRES, 1.23);
    EEPROMWriteFloat(EEPROM_PRESET2_ADDRES, 4.56);
    EEPROMWriteFloat(EEPROM_PRESET3_ADDRES, 7.89);
    EEPROMWriteFloat(EEPROM_PRESET4_ADDRES, 10.11);
    EEPROMWriteFloat(EEPROM_PRESET5_ADDRES, -11);

    EEPROM.write(EEPROM_FLAG_ADDRESS, 0xAA);
    EEPROM.commit();
  }
}

void init_eeprom() {
  if (!EEPROM.begin(EEPROM_SIZE + 1)) { Serial.println("EEPROM failed to initialise"); }
  EEPROMWipeOnce();
}

void wczytaj_presety() {
  preset[0] = EEPROMReadFloat(EEPROM_PRESET1_ADDRES);
  preset[1] = EEPROMReadFloat(EEPROM_PRESET2_ADDRES);
  preset[2] = EEPROMReadFloat(EEPROM_PRESET3_ADDRES);
  preset[3] = EEPROMReadFloat(EEPROM_PRESET4_ADDRES);
  preset[4] = EEPROMReadFloat(EEPROM_PRESET5_ADDRES);
}
void init_on_reset() {
  init_eeprom();
  wczytaj_presety();
}
