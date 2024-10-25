#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// Configurația rețelei Wi-Fi
const char* ssid = "RACE-DEV.COM";
const char* parola = "tuning123?";

// Configurație ecran OLED
#define LATIME_ECRAN 128
#define INALTIME_ECRAN 64
#define RESET_OLED -1
Adafruit_SSD1306 ecran(LATIME_ECRAN, INALTIME_ECRAN, &Wire, RESET_OLED);

// Configurație DHT22
#define PIN_DHT D4
#define TIP_DHT DHT22
DHT dht(PIN_DHT, TIP_DHT);

// Configurație releu și pini
#define PIN_RELEU D5

// Variabile pentru controlul temperaturii
float temperaturaCurenta = 0.0;
float temperaturaSetata = 19.0;  // Temperatura implicită
bool esteIncalzirePornita = false;  // Starea centralei (pornit/oprit)

// Server web
ESP8266WebServer server(80);

// Funcția pentru a porni încălzirea
void pornesteIncalzirea() {
  digitalWrite(PIN_RELEU, HIGH);  // Activează releul
  esteIncalzirePornita = true;
}

// Funcția pentru a opri încălzirea
void opresteIncalzirea() {
  digitalWrite(PIN_RELEU, LOW);   // Dezactivează releul
  esteIncalzirePornita = false;
}

// Funcție pentru afișarea temperaturii pe ecranul OLED
void afiseazaTemperatura() {
  ecran.clearDisplay();
  ecran.setTextSize(1);
  ecran.setTextColor(SSD1306_WHITE);
  ecran.setCursor(0, 0);
  ecran.print("Temperatura curenta:");

  // Afișează temperatura curentă mai mare
  ecran.setTextSize(3);
  ecran.setCursor(0, 16);
  ecran.print(temperaturaCurenta, 1);
  ecran.print(" C");

  // Afișează temperatura setată mai jos
  ecran.setTextSize(1);
  ecran.setCursor(0, 48);
  ecran.print("Setat la: ");
  ecran.print(temperaturaSetata, 1);
  ecran.print(" C");

  // Afișează starea încălzirii
  ecran.setCursor(0, 56);
  ecran.print("Incalzire: ");
  ecran.print(esteIncalzirePornita ? "Pornit" : "Oprit");
  ecran.display();
}

// Funcția pentru afișarea mesajului de pornire
void afiseazaMesajPornire() {
  ecran.clearDisplay();
  ecran.setTextSize(2);
  ecran.setTextColor(SSD1306_WHITE);
  ecran.setCursor(0, 24);
  ecran.print("TERO-PULA-MEA");
  ecran.display();
  delay(6000);  // Afișează mesajul timp de 3 secunde
  ecran.clearDisplay();
}

// Actualizează temperatura și controlează starea încălzirii
void actualizeazaTemperatura() {
  temperaturaCurenta = dht.readTemperature();
  delay(2000); // Așteaptă 2 secunde
  // Verifică dacă citirea temperaturii este validă
  if (isnan(temperaturaCurenta)) {
    Serial.println("Eroare la citirea temperaturii!");
    return;
  }

  // Controlează încălzirea pe baza temperaturii setate și diferență de 1.5 grade
  if (temperaturaCurenta < temperaturaSetata - 1.0) {
    pornesteIncalzirea();
  } else if (temperaturaCurenta >= temperaturaSetata + 1.0) {
    opresteIncalzirea();
  }
}

// Funcția pentru afișarea paginii principale în interfața web
void afisarePaginaPrincipala() {
  String html = "<!DOCTYPE html><html><head><title>Control Termostat</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>"; 
  html += "<link rel='stylesheet' href='https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha1/dist/css/bootstrap.min.css'>"; 
  html += "<script>setInterval(function(){fetch('/temperatura').then(resp => resp.text()).then(data => {document.getElementById('temp').innerHTML = data;});}, 5000);</script></head><body>";
  html += "<div class='container mt-5'><h2>Control Termostat Smart</h2>";
  html += "<p>Temperatura curenta: <strong id='temp'>" + String(temperaturaCurenta, 1) + " C</strong></p>";
  html += "<p>Temperatura setata: <strong>" + String(temperaturaSetata, 1) + " C</strong></p>";
  html += "<form action='/seteaza' method='get' onsubmit='setTimeout(function(){location.reload();},500);'>";
  html += "<div class='mb-3'><label class='form-label'>Seteaza temperatura:</label>";
  html += "<input type='number' name='temperatura' step='0.5' min='10' max='30' value='" + String(temperaturaSetata, 1) + "' class='form-control'></div>";
  html += "<button type='submit' class='btn btn-primary'>Seteaza</button></form>";
  html += "<p>Status incalzire: <strong>" + String(esteIncalzirePornita ? "Pornit" : "Oprit") + "</strong></p>";
  html += "</div></body></html>";
  
  server.send(200, "text/html", html);
}

// Funcția pentru setarea temperaturii din interfața web
void seteazaTemperatura() {
  if (server.hasArg("temperatura")) {
    temperaturaSetata = server.arg("temperatura").toFloat();
    server.sendHeader("Location", "/");
    server.send(303);
  } else {
    server.send(400, "text/plain", "Temperatura nu a fost setata corect.");
  }
}

// Trimite temperatura curenta la solicitare
void trimiteTemperatura() {
  server.send(200, "text/plain", String(temperaturaCurenta, 1) + " C");
}

// Configurarea inițială
void setup() {
  Serial.begin(115200);
  
  // Configurare pini
  pinMode(PIN_RELEU, OUTPUT);
  opresteIncalzirea();  // Oprește încălzirea inițial

  // Configurare Wi-Fi
  WiFi.begin(ssid, parola);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectat la Wi-Fi");
  Serial.print("IP local: ");
  Serial.println(WiFi.localIP());

  // Configurare server web
  server.on("/", afisarePaginaPrincipala);
  server.on("/seteaza", seteazaTemperatura);
  server.on("/temperatura", trimiteTemperatura);
  server.begin();
  Serial.println("Serverul web este pornit.");

  // Inițializare ecran OLED
  if (!ecran.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Nu s-a putut initializa OLED"));
    for (;;) ;
  }
  afiseazaMesajPornire();  // Afișează mesajul la pornire
  
  // Inițializare DHT22
  dht.begin();
}

// Bucla principală
void loop() {
  server.handleClient();    // Gestionare cereri HTTP
  actualizeazaTemperatura();  // Actualizează temperatura și controlează încălzirea
  afiseazaTemperatura();      // Afișare temperatură pe OLED
  delay(2000);                // Interval de actualizare
}
