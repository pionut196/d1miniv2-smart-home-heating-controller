#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>

const char* ssid = "RACE-DEV.COM";
const char* parola = "tuning123?";

#define LATIME_ECRAN 128
#define INALTIME_ECRAN 64
#define RESET_OLED -1
Adafruit_SSD1306 ecran(LATIME_ECRAN, INALTIME_ECRAN, &Wire, RESET_OLED);

#define PIN_DHT D4
#define TIP_DHT DHT22
DHT dht(PIN_DHT, TIP_DHT);

#define PIN_RELEU D5

float temperaturaCurenta = 0.0;
float temperaturaSetata = 19.0;
bool esteIncalzirePornita = false;
bool ecoMode = false;

ESP8266WebServer server(80);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7200, 60000);

String textAnimatie = "";
bool animatiePornita = false;
int xText = LATIME_ECRAN;

unsigned long lastIgnitionTime = 0;
const unsigned long antiCyclingDelay = 120000; // 120 seconds

// Function to start heating with anti-cycling control
void pornesteIncalzirea() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastIgnitionTime > antiCyclingDelay || lastIgnitionTime == 0) {
    digitalWrite(PIN_RELEU, HIGH);
    esteIncalzirePornita = true;
    lastIgnitionTime = currentMillis;
  }
}

// Function to stop heating
void opresteIncalzirea() {
  digitalWrite(PIN_RELEU, LOW);
  esteIncalzirePornita = false;
}

// Display current temperature on OLED
void afiseazaTemperatura() {
  ecran.clearDisplay();
  ecran.setTextSize(1);
  ecran.setTextColor(SSD1306_WHITE);
  ecran.setCursor(0, 0);
  ecran.print("Temperatura curenta:");

  ecran.setTextSize(3);
  ecran.setCursor(0, 16);
  ecran.print(temperaturaCurenta, 1);
  ecran.print(" C");

  ecran.setTextSize(1);
  ecran.setCursor(0, 48);
  ecran.print("Setat la: ");
  ecran.print(temperaturaSetata, 1);
  ecran.print(" C");

  ecran.setCursor(0, 56);
  ecran.print("Incalzire: ");
  ecran.print(esteIncalzirePornita ? "Pornit" : "Oprit");
  ecran.display();
}

// Update temperature with DHT22 sensor readings and control heating
void actualizeazaTemperatura() {
  float temperaturaNoua = dht.readTemperature();

  if (!isnan(temperaturaNoua)) {
    temperaturaCurenta = temperaturaNoua;

    // Heating control thresholds with anti-cycling check
    if (temperaturaCurenta < temperaturaSetata - 0.5 && !esteIncalzirePornita) {
      pornesteIncalzirea();
    } else if (temperaturaCurenta >= temperaturaSetata && esteIncalzirePornita) {
      opresteIncalzirea();
    }
  }
}

// Function to start animation
void pornesteAnimatie() {
  if (server.hasArg("text")) {
    textAnimatie = server.arg("text");
    animatiePornita = true;
    xText = LATIME_ECRAN;
    server.sendHeader("Location", "/");
    server.send(303);
  } else {
    server.send(400, "text/plain", "Textul nu a fost furnizat.");
  }
}

// Function to stop animation
void opresteAnimatie() {
  animatiePornita = false;
  server.sendHeader("Location", "/");
  server.send(303);
}

// Animation display function on OLED
void afiseazaAnimatie() {
  ecran.clearDisplay();
  ecran.setTextSize(2);
  ecran.setTextColor(SSD1306_WHITE);
  ecran.setCursor(xText, 24);
  ecran.print(textAnimatie);
  ecran.display();

  xText -= 2;
  if (xText < -((int)textAnimatie.length() * 12)) {
    xText = LATIME_ECRAN;
  }
}

// Function to generate HTML page content
String genereazaHTML() {
  String html = "<!DOCTYPE html><html><head><title>Control Termostat</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<link rel='stylesheet' href='https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha1/dist/css/bootstrap.min.css'>";
  html += "<script>setInterval(function(){fetch('/temperatura').then(resp => resp.text()).then(data => {document.getElementById('temp').innerHTML = data;});}, 5000);</script>";
  html += "<script>function updateSlider(value) { document.getElementById('sliderValue').innerText = value + ' C'; }</script></head><body>";
  html += "<div class='container mt-5'><h2>Control Termostat Smart</h2>";
  html += "<p>Temperatura curenta: <strong id='temp'>" + String(temperaturaCurenta, 1) + " C</strong></p>";
  html += "<p>Temperatura setata: <strong>" + String(temperaturaSetata, 1) + " C</strong></p>";
  
  // Slider for setting temperature
  html += "<form action='/seteaza' method='get'><div class='mb-3'><label class='form-label'>Seteaza temperatura:</label>";
  html += "<input type='range' name='temperatura' min='15' max='30' value='" + String(temperaturaSetata, 1) + "' class='form-range' onchange='updateSlider(this.value)' oninput='updateSlider(this.value)'>";
  html += "<span id='sliderValue'>" + String(temperaturaSetata, 1) + " C</span></div>";
  html += "<button type='submit' class='btn btn-primary'>Seteaza</button></form>";
  
  html += "<p>Status incalzire: <strong>" + String(esteIncalzirePornita ? "Pornit" : "Oprit") + "</strong></p>";
  html += "<p>Data si ora: <strong>" + timeClient.getFormattedTime() + "</strong></p>";
  html += "<p>Mod Eco: <strong>" + String(ecoMode ? "Activat" : "Dezactivat") + "</strong></p>";
  
  html += "<hr><h3>Animatie Text</h3>";
  html += "<form action='/start_animatie' method='get'><div class='mb-3'><label class='form-label'>Text pentru animatie:</label>";
  html += "<input type='text' name='text' class='form-control' placeholder='Introduceti textul'></div>";
  html += "<button type='submit' class='btn btn-success'>Start Animatie</button></form>";
  html += "<form action='/stop_animatie' method='get' style='margin-top:10px;'><button type='submit' class='btn btn-danger'>Stop Animatie</button></form>";
  html += "</div></body></html>";
  return html;
}

// Display main page on web server
void afisarePaginaPrincipala() {
  server.send(200, "text/html", genereazaHTML());
}

// Function to send current temperature to web interface
void trimiteTemperatura() {
  server.send(200, "text/plain", String(temperaturaCurenta, 1) + " C");
}

void seteazaTemperatura() {
  if (server.hasArg("temperatura")) {
    temperaturaSetata = server.arg("temperatura").toFloat();
    server.sendHeader("Location", "/");
    server.send(303);

    if (temperaturaCurenta >= temperaturaSetata && esteIncalzirePornita) {
      opresteIncalzirea();
    } else if (temperaturaCurenta < temperaturaSetata - 0.5 && !esteIncalzirePornita) {
      pornesteIncalzirea();
    }
  } else {
    server.send(400, "text/plain", "Temperatura nu a fost setata corect.");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_RELEU, OUTPUT);
  opresteIncalzirea();

  WiFi.begin(ssid, parola);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectat la Wi-Fi");
  Serial.print("IP local: ");
  Serial.println(WiFi.localIP());

  server.on("/", afisarePaginaPrincipala);
  server.on("/seteaza", seteazaTemperatura);
  server.on("/temperatura", trimiteTemperatura);
  server.on("/start_animatie", pornesteAnimatie);
  server.on("/stop_animatie", opresteAnimatie);
  server.begin();

  timeClient.begin();
  dht.begin();

  ecran.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  afiseazaTemperatura();
}

void loop() {
  server.handleClient();
  actualizeazaTemperatura();
  timeClient.update();

  if (animatiePornita) {
    afiseazaAnimatie();
  } else {
    afiseazaTemperatura();
  }
}
