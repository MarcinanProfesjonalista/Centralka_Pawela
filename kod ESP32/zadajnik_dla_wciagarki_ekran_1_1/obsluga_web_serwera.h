#include "WString.h"

#include <WebServer.h>
#include <WebSocketsServer.h>

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

TaskHandle_t websocketTaskHandle = NULL;

String JSONtxt;
//----------------------------------------------- Strona internetowa
#include "webpage_index.h"
#include "webpage_manual.h"


// float mock = 0.0;
String przygotuj_jazona() {
// mock=mock+0.01;
// // Serial.println(mock);
// aktualna_pozycja = aktualna_pozycja + 0.1;
// if(aktualna_pozycja > 14)aktualna_pozycja = 0.0;

  String wynik = "";
  wynik += "{";
  wynik += "\"id\":" + String(id) + ",";
  wynik += "\"akt\":" + String(aktualna_pozycja, 2) + ",";
  //wynik += "\"cel\":" + String(targetPosition, 2) + ",";
  wynik += "\"krg\":" + String(krGorna ? 1 : 0) + ",";
  wynik += "\"krd\":" + String(krDolna ? 1 : 0) + ",";
  wynik += "\"czy_ma_jechac\":" + String(czy_ma_jechac ? 1 : 0) + ",";
  wynik += "\"czy_mam_komunikacje\":" + String(czy_mam_komunikacje ? 1 : 0);//+ ",";
  // wynik += "\"kierunek_jazdy\":" + String(czy_mam_komunikacje ? 1 : 0);
  wynik += "}";
  wynik += "";
  return wynik;
}

void handleSend() {
  String kontent = "";

  // Pobranie parametrów
  String idStr = server.arg("id");
  String docelowaStr = server.arg("docelowa");
  String sterowanieStr = server.arg("sterowanie");

  // Sprawdzenie czy wszystkie argumenty są obecne i niepuste
  if (idStr.length() > 0 && docelowaStr.length() > 0 && sterowanieStr.length() > 0) {
    // Sprawdzenie czy id i sterowanie to liczby całkowite, a docelowa to liczba zmiennoprzecinkowa
    bool idOK = idStr.equals(String(idStr.toInt()));
    bool sterowanieOK = sterowanieStr.equals(String(sterowanieStr.toInt()));
    
    char* endptr;
    float docelowaTest = strtof(docelowaStr.c_str(), &endptr);
    bool docelowaOK = (*endptr == '\0');

    if (idOK && sterowanieOK && docelowaOK) {
      // Tworzenie zmiennych wynikowych
      int id_wybrane = idStr.toInt();
      float docelowa_wartosc_wybrana = docelowaTest;
      int sterowanie_wybrane = sterowanieStr.toInt();

      // Wywołanie funkcji sterującej
      steruj_wciagarka(id_wybrane, docelowa_wartosc_wybrana, sterowanie_wybrane);

      // Odpowiedź dla klienta
      kontent = "Dane przetworzone <meta http-equiv = 'refresh' content = '3; url = " + String(WiFi.localIP()) + " />";
      server.send(200, "text/html", webpageContIndex);
      return;
    }
  }

  // Jeśli nie przeszło sanityzacji — odpowiedź błędna
  kontent = "<meta http-equiv = 'refresh' content = '5; url = " + String(WiFi.localIP()) + " /><br>Ojej. Zbiornik pełny.";
  Serial.println("Brak identyfikatora czujnika");
  server.send(400, "text/html", kontent);
}

void handleRoot() {
  server.send(200, "text/html", webpageContIndex);
}

void handleManual() {
  server.send(200, "text/html", webpageContManual);
}


unsigned long previousMillis = 0;  // Zmienna do przechowywania poprzedniego czasu
const long interval = 1000;

void obsluga_websocketu() {
  unsigned long currentMillis = millis();  // Pobierz aktualny czas
  server.handleClient();
  webSocket.loop();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    String DANE = przygotuj_jazona();
    JSONtxt = "{\"DANE\": " + DANE + "}";
    webSocket.broadcastTXT(JSONtxt);
  }
}

void obsluga_websocketuTask(void* parameter) {
  for (;;) {
    obsluga_websocketu();  // wykonuje się cyklicznie
    vTaskDelay(100 / portTICK_PERIOD_MS);  // sprawdzanie co 100ms
  }
}


void init_all_web_pages() {
  server.on(toStringIp(apIP), handleRoot);
  server.on("/", handleRoot);
  server.on("/instrukcja", handleManual);
  server.on("/steruj", HTTP_GET, handleSend);
  server.begin();
  webSocket.begin();

    // uruchomienie wątku websocketu
  xTaskCreatePinnedToCore(
    obsluga_websocketuTask, // funkcja taska
    "WebSocketTask",        // nazwa
    4096,                   // stos
    NULL,                   // parametry
    1,                      // priorytet
    &websocketTaskHandle,   // uchwyt
    1                       // rdzeń (np. 1)
  );
}


