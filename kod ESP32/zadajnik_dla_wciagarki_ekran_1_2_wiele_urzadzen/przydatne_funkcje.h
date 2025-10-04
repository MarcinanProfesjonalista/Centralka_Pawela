#include <EEPROM.h>

#define EEPROM_SIZE 100                 //ostatni adres użyty + 9
const uint8_t EEPROM_FLAG_ADDRESS = 0;  //Flaga użyta tylko raz.
const uint8_t eeprom_id_address = 1;    //jakos byte

#define liczba_czujnikow 64

#ifdef ARDUINO_ARCH_ESP32
// Kod specyficzny dla ESP32

// Dodatkowe komendy dla ESP32
#define EEPROM_COMMIT() EEPROM.commit()
#endif

#define EEPROM_id_windy_z_ktora_ma_teraz_gadac 50

#define EEPROM_PRESET1_ADDRES 10
#define EEPROM_PRESET2_ADDRES 14
#define EEPROM_PRESET3_ADDRES 18
#define EEPROM_PRESET4_ADDRES 22
#define EEPROM_PRESET5_ADDRES 26


void zapisz_id_windy_z_ktora_ma_teraz_gadac(byte id) {
  EEPROM.write(EEPROM_id_windy_z_ktora_ma_teraz_gadac, id);
  EEPROM.commit();
}

byte wczytaj_id_windy_z_ktora_ma_teraz_gadac() {
  
  return EEPROM.read(EEPROM_id_windy_z_ktora_ma_teraz_gadac);
}

long readBinaryString(char *s) {
  long result = 0;
  while (*s) {
    result <<= 1;
    if (*s++ == '1')
      result |= 1;
  }
  return result;
}

long binaryStringToLong(String binaryString) {
  char charArray[binaryString.length() + 1];
  binaryString.toCharArray(charArray, binaryString.length() + 1);
  long result = readBinaryString(charArray);
  return result;
}

long EEPROMReadlong(long address) {
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);

  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

void EEPROMWritelong(int address, long value) {
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);

  EEPROM.write(address, four);
  EEPROM_COMMIT();
  EEPROM.write(address + 1, three);
  EEPROM_COMMIT();
  EEPROM.write(address + 2, two);
  EEPROM_COMMIT();
  EEPROM.write(address + 3, one);
  EEPROM_COMMIT();
}

double EEPROMReadDouble(long address) {
  union {
    double value;
    byte bytes[8];
  } data;

  // Odczyt bajtów z EEPROM
  for (int i = 0; i < 8; i++) {
    data.bytes[i] = EEPROM.read(address + i);
  }

  return data.value;
}

void EEPROMWriteDouble(long address, double value) {
  union {
    double value;
    byte bytes[8];
  } data;

  data.value = value;

  // Zapis bajtów do EEPROM
  for (int i = 0; i < 8; i++) {
    EEPROM.write(address + i, data.bytes[i]);
  }

  EEPROM_COMMIT();
}

float EEPROMReadFloat(long address) {
  byte four = EEPROM.read(address);
  byte three = EEPROM.read(address + 1);
  byte two = EEPROM.read(address + 2);
  byte one = EEPROM.read(address + 3);

  union {
    float value;
    byte bytes[4];
  } data;

  data.bytes[0] = four;
  data.bytes[1] = three;
  data.bytes[2] = two;
  data.bytes[3] = one;

  return data.value;
}

void EEPROMWriteFloat(long address, float value) {
  union {
    float value;
    byte bytes[4];
  } data;

  data.value = value;

  EEPROM.write(address, data.bytes[0]);
  EEPROM.write(address + 1, data.bytes[1]);
  EEPROM.write(address + 2, data.bytes[2]);
  EEPROM.write(address + 3, data.bytes[3]);

  EEPROM_COMMIT();  // Potwierdzenie zapisu w pamięci EEPROM
}

int EEPROMReadInt(long address) {
  byte highByte = EEPROM.read(address);
  byte lowByte = EEPROM.read(address + 1);

  return (highByte << 8) | lowByte;
}

void EEPROMWriteInt(long address, int value) {
  byte highByte = (value >> 8) & 0xFF;
  byte lowByte = value & 0xFF;

  EEPROM.write(address, highByte);
  EEPROM.write(address + 1, lowByte);

  EEPROM.commit();  // Potwierdzenie zapisu w pamięci EEPROM
}

long stringToLong(String str) {
  long result = 0;
  int sign = 1;
  int i = 0;
  if (str.charAt(0) == '-') {
    sign = -1;
    i++;
  }

  for (; i < str.length(); i++) {
    result = (result * 10) + (str.charAt(i) - '0');
  }

  return result * sign;
}
bool isNumber(String str) {
  for (int i = 0; i < str.length(); i++) {
    if (!isdigit(str.charAt(i))) {
      return false;
    }
  }
  return true;
}
String longToBinaryString(long num) {
  String result = "";
  for (int i = 31; i >= 0; i--) {
    if ((num >> i) & 1) {
      result += "1";
    } else {
      result += "0";
    }
  }
  return result;
}

String Long_to_a(long number) {
  String result = "";  // Inicjalizacja pustego napisu

  if (number == 0) {
    // Dla liczby zero zwracamy "0" jako napis
    result = "0";
    return result;
  }

  bool isNegative = false;

  if (number < 0) {
    isNegative = true;
    number = -number;  // Zamiana liczby ujemnej na dodatnią
  }

  // Konwertujemy liczbę na napis, wykonując operację modulo 10 na kolejnych cyfrach
  while (number > 0) {
    char digit = (char)(number % 10) + '0';  // Konwersja cyfry na znak ASCII
    result = digit + result;                 // Dodajemy cyfrę na początek napisu
    number /= 10;                            // Usuwamy ostatnią cyfrę z liczby
  }

  if (isNegative) {
    result = "-" + result;  // Dodaj minus, jeśli liczba była ujemna
  }

  return result;
}

String toStringIp(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}