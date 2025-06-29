#include "HardwareSerial.h"
//#include <ModbusRTU.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <CSE_ArduinoRS485.h>
#include <CSE_ModbusRTU.h>
bool modbusInitialized = false;  // Flaga inicjalizacji



//===================================================================================//

// You can define the serial port pins here.
#define PIN_RS485_RX 16  //16 //Nowa płyta RX 16, tx17 en15
#define PIN_RS485_TX 17  //17

#define PORT_RS485 Serial1  // The hardware serial port for the RS-485 interface

RS485Class RS485(PORT_RS485, -1, 15, PIN_RS485_TX);
CSE_ModbusRTU modbusRTU(&RS485, 0x01, "modbusRTU-0x01");
CSE_ModbusRTU_Client modbusRTUClient(modbusRTU, "modbusRTUClient");


// Indeksy rejestrów
#define REG_ID 0
#define REG_ZEZWOL_NA_JAZDE 1  //1 rejestr
#define REG_TARGET_POSITION 2
#define REG_ACTUAL_POSITION 3
#define REG_LIMIT_TOP_SWITCH 4
#define REG_LIMIT_BOT_SWITCH 5
#define REG_ERROR 6                  //1 rejestr
#define REG_ilosc_liny_na_bobinie 7  //
#define REG_krokow_enkodera_na_metr 8

unsigned long ostatni_udany_odczyt = 0;  // Czas ostatniego udanego odczytu w ms

void steruj_wciagarka(int id_wybrane, float docelowa_wartosc_wybrana, int sterowanie_wybrane){
czy_ma_jechac = sterowanie_wybrane;
targetPosition = docelowa_wartosc_wybrana;
//Ta funkcja zrobiona jest na odpierdol. Ma działać tylko dla jednej wciagarki. 
}

void obsluga_modbus_rtu() {
  // Wysyłanie danych do serwera
  modbusRTUClient.writeHoldingRegister(1, czy_ma_jechac ? 1 : 0);
  modbusRTUClient.writeHoldingRegister(2, int(targetPosition * 1000));

  // Odbieranie danych z serwera
  uint16_t holdingRegisters[3] = { 0 };

  if (modbusRTUClient.readHoldingRegister(3, 3, holdingRegisters) != -1) {
    // Odczyt udany
    aktualna_pozycja = float((int16_t)holdingRegisters[0]) / 1000.0;

    krGorna = (holdingRegisters[1] == 1);
    krDolna = (holdingRegisters[2] == 1);

    // Aktualizacja czasu ostatniego udanego odczytu
    ostatni_udany_odczyt = millis();
    czy_mam_komunikacje = true;

  } else {
    // Jeśli minęło więcej niż 5000 ms od ostatniego udanego odczytu
    if (millis() - ostatni_udany_odczyt > 5000) {
      czy_mam_komunikacje = false;
    }
  }
}

void ModbusTask(void *pvParameters) {
  for (;;) {
    obsluga_modbus_rtu();                 // Wywołanie komunikacji Modbus w pętli
    vTaskDelay(100 / portTICK_PERIOD_MS);  // Opóźnienie 10 ms
  }
}

void init_modbus_rtu() {
  //Serial1.begin(9600);
  PORT_RS485.begin(9600, SERIAL_8N1, PIN_RS485_RX, PIN_RS485_TX);
  RS485.begin();
  modbusRTUClient.begin();
  modbusRTUClient.setServerAddress(0x01);
  CSE_ModbusRTU_Debug::disableDebugMessages();
    xTaskCreatePinnedToCore(
    ModbusTask,     // Funkcja tasku
    "Modbus Task",  // Nazwa tasku
    4000,           // Rozmiar stosu
    NULL,           // Parametry tasku
    1,              // Priorytet tasku
    NULL,           // Uchwyt tasku
    1               // Przypisanie tasku do rdzenia 1 (drugi rdzeń)
  );
}

