---

# 🎤 Wciągarka Mikrofonowa – System Sterowania

**Projekt demonstracyjny prezentujący możliwości wciągarki mikrofonowej sterowanej protokołem Modbus RTU.**

---

## 📦 Opis projektu

Układ ten stanowi **centralę sterującą** dla wciągarki mikrofonowej, zaprojektowaną do celów pokazowych i testowych. Umożliwia zarówno **monitorowanie**, jak i **kontrolę** pozycji liny wciągarki poprzez interfejs Modbus RTU (przez złącze 3-pinowe DMX).

---

## ⚙️ Parametry komunikacji

* **Protokół:** Modbus RTU
* **Złącze:** 3-pinowe DMX
* **Baudrate:** 9600
* **Format:** 8N1 (8 bitów danych, brak parzystości, 1 bit stopu)

---

## 📚 Rejestry Modbus

| Adres | Nazwa                  | Opis                                    |
| ----- | ---------------------- | --------------------------------------- |
| `0`   | `REG_ID`               | ID urządzenia                           |
| `1`   | `REG_ZEZWOL_NA_JAZDE`  | Zezwolenie na ruch (1 = tak, 0 = nie)   |
| `2`   | `REG_TARGET_POSITION`  | Pozycja docelowa (liczba kroków/pulsów) |
| `3`   | `REG_ACTUAL_POSITION`  | Aktualna pozycja wciągarki              |
| `4`   | `REG_LIMIT_TOP_SWITCH` | Krańcówka górna (1 = aktywna)           |
| `5`   | `REG_LIMIT_BOT_SWITCH` | Krańcówka dolna (1 = aktywna)           |

---

## 🔌 Instrukcja użycia

1. **Połącz** centralę sterującą z wciągarką przy pomocy przewodu DMX.
2. **Podłącz zasilanie** do centralki (zakres napięcia: **7V – 36V**).
3. Po uruchomieniu centrala:

   * automatycznie łączy się z wciągarką,
   * umożliwia **podgląd aktualnego stanu** urządzenia (pozycja, krańcówki),
   * pozwala na **ustawienie docelowej pozycji liny** (wypchnięcie lub wciągnięcie).

---

## 🛠️ Zastosowanie

Ten projekt może być używany m.in. do:

* demonstracji działania systemu scenicznego,
* testów działania silnika i krańcówek,
* wizualizacji sterowania protokołem Modbus RTU.

---

## 🧪 Status projektu

✅ Projekt gotowy do użytku pokazowego.
🔧 Dalszy rozwój możliwy w kierunku rozbudowy GUI i integracji z systemami scenicznymi.

---
