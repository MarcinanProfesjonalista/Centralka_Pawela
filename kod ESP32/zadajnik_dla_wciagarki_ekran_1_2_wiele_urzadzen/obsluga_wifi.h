#include <WiFi.h>
#include <esp_wifi.h>

//-----------------------------------------------
// Ustawienia sieci Wi-Fi (tryb Access Point)
static String ssid = "MikorWciagarka";
static String password = "Mikor123";

// Adres IP, brama i maska podsieci dla AP
IPAddress apIP(192, 168, 0, 1);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

void init_wifi() {
  // Ustaw tryb Wi-Fi na Access Point
  WiFi.mode(WIFI_AP);

  // Skonfiguruj adres IP Access Pointa
  bool configSuccess = WiFi.softAPConfig(apIP, gateway, subnet);
  if (!configSuccess) {
    Serial.println("Błąd konfiguracji IP dla Access Pointa!");
    return;
  }

  // Uruchom Access Point z nazwą i hasłem
  bool apStarted = WiFi.softAP(ssid.c_str(), password.c_str());
  if (apStarted) {
    Serial.println("Access Point uruchomiony");
    Serial.print("IP AP: ");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("Nie udało się uruchomić Access Pointa!");
  }
}