#include "HardwareSerial.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <CSE_ArduinoRS485.h>
#include <CSE_ModbusRTU.h>

bool modbusInitialized = false;  // Flaga inicjalizacji

//===================================================================================//
// Definicje pinów

#define PIN_RS485_TX 17
#define PIN_RS485_RX 16
#define PIN_RS485_EN 4

#define PORT_RS485 Serial1

// Inicjalizacja RS485 i Modbus RTU
RS485Class RS485(PORT_RS485, PIN_RS485_EN, -1, PIN_RS485_TX);
CSE_ModbusRTU modbusRTU(&RS485, 0x01, "modbusRTU-0x01");
CSE_ModbusRTU_Client modbusRTUClient(modbusRTU, "modbusRTUClient");

// Adresy rejestrów
#define REG_ID 0
#define REG_ZEZWOL_NA_JAZDE 1
#define REG_TARGET_POSITION 2
#define REG_ACTUAL_POSITION 3
#define REG_LIMIT_TOP_SWITCH 4
#define REG_LIMIT_BOT_SWITCH 5
#define REG_ZEZWOL_NA_JAZDE_DUPLIKACJA 6
#define REG_TARGET_POSITION_od_podlogi 7
#define REG_POZYCJA_MIKROFONU_LICZONA_OD_PODLOGI 8

// Zmienne robocze
unsigned long ostatni_udany_odczyt = 0;
int czy_ma_jechac = 1;
int targetPosition_od_podlogi = 2000;

//===================================================================================//

void steruj_wciagarka(int id_wybrane, float docelowa_wartosc_wybrana, int sterowanie_wybrane) {
  czy_ma_jechac = sterowanie_wybrane;
  targetPosition_od_podlogi = docelowa_wartosc_wybrana;
  // Funkcja tymczasowa – działa tylko dla jednej wciągarki
}

void obsluga_modbus_rtu_zadajnik() {
  targetPosition_od_podlogi++;
  // Zapisz dane do serwera (duplikaty)
  modbusRTUClient.writeHoldingRegister(REG_ZEZWOL_NA_JAZDE_DUPLIKACJA, czy_ma_jechac ? 1 : 0);
  modbusRTUClient.writeHoldingRegister(REG_TARGET_POSITION_od_podlogi, targetPosition_od_podlogi);

  // Odczytaj 9 rejestrów (od REG_ID = 0 do REG_POZYCJA_MIKROFONU_LICZONA_OD_PODLOGI = 8)
  uint16_t holdingRegisters[9] = { 0 };
  if (modbusRTUClient.readHoldingRegister(REG_ID, 9, holdingRegisters)) {
    ostatni_udany_odczyt = millis();

    Serial.println("========== Odczyt rejestrów Modbus ==========");
    Serial.print("Czas: ");
    Serial.print(ostatni_udany_odczyt);
    Serial.println(" ms");
    Serial.print("REG_ID: ");
    Serial.println(holdingRegisters[0]);
    Serial.print("REG_ZEZWOL_NA_JAZDE: ");
    Serial.println(holdingRegisters[1]);
    Serial.print("REG_TARGET_POSITION: ");
    Serial.println(holdingRegisters[2]);
    uint16_t reg = holdingRegisters[3];  // np. 63202
    int16_t signedValue = (int16_t)reg;  // rzutowanie na signed

    Serial.print("REG_ACTUAL_POSITION: ");
    Serial.println(signedValue);  // teraz wyświetli prawidłową wartość ujemną
    Serial.print("REG_LIMIT_TOP_SWITCH: ");
    Serial.println(holdingRegisters[4]);
    Serial.print("REG_LIMIT_BOT_SWITCH: ");
    Serial.println(holdingRegisters[5]);
    Serial.print("REG_ZEZWOL_NA_JAZDE_DUPLIKACJA: ");
    Serial.println(holdingRegisters[6]);
    Serial.print("REG_TARGET_POSITION_od_podlogi: ");
    Serial.println(holdingRegisters[7]);
    Serial.print("REG_POZYCJA_MIKROFONU_LICZONA_OD_PODLOGI: ");
    Serial.println(holdingRegisters[8]);

    Serial.println("============================================");
  } else {
    Serial.println("❌ Błąd odczytu rejestrów Modbus!");
  }
}

//===================================================================================//

void ModbusTask(void *pvParameters) {
  for (;;) {
    obsluga_modbus_rtu_zadajnik();
    vTaskDelay(500 / portTICK_PERIOD_MS);  // co 0.5 sekundy
  }
}

//===================================================================================//

void init_modbus_rtu() {
  Serial.begin(115200);
  PORT_RS485.begin(38400, SERIAL_8N1, PIN_RS485_TX, PIN_RS485_RX);
  RS485.begin();
  modbusRTUClient.begin();
  modbusRTUClient.setServerAddress(3);
  CSE_ModbusRTU_Debug::disableDebugMessages();

  xTaskCreatePinnedToCore(
    ModbusTask,
    "Modbus Task",
    4000,
    NULL,
    1,
    NULL,
    1  // rdzeń 1
  );

  modbusInitialized = true;
  Serial.println("✅ Modbus RTU zainicjalizowany!");
}
