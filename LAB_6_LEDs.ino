#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "Galaxy A25 5G E50E";
const char* password = "11111111";

const char* mdnsName = "Me_005_esp32";

WebServer server(80);

// LED pins
#define LED1 2   // D2
#define LED2 4   // D4

// Store LED states
bool led1State = false;
bool led2State = false;

// HTML Page 
void handleRoot() {

  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>";
  html += "body { font-family: Arial; text-align: center; margin-top: 50px; }";
  html += "button { padding: 15px 30px; font-size: 18px; margin: 10px; }";
  html += "</style></head><body>";

  html += "<h1>ESP32 LED Control</h1>";

  html += "<p>LED 1 (GPIO2)</p>";
  html += "<a href='/led1on'><button>ON</button></a>";
  html += "<a href='/led1off'><button>OFF</button></a>";

  html += "<p>LED 2 (GPIO4)</p>";
  html += "<a href='/led2on'><button>ON</button></a>";
  html += "<a href='/led2off'><button>OFF</button></a>";

  html += "</body></html>";

  server.send(200, "text/html", html);
}

// LED Handlers
void led1On() {
  digitalWrite(LED1, HIGH);
  led1State = true;
  server.sendHeader("Location", "/");
  server.send(303);
}

void led1Off() {
  digitalWrite(LED1, LOW);
  led1State = false;
  server.sendHeader("Location", "/");
  server.send(303);
}

void led2On() {
  digitalWrite(LED2, HIGH);
  led2State = true;
  server.sendHeader("Location", "/");
  server.send(303);
}

void led2Off() {
  digitalWrite(LED2, LOW);
  led2State = false;
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());

  if (!MDNS.begin(mdnsName)) {
    Serial.println("Error setting up MDNS!");
    while (1) { delay(1000); }
  }

  MDNS.addService("_http", "_tcp", 80);

  // Routes
  server.on("/", handleRoot);
  server.on("/led1on", led1On);
  server.on("/led1off", led1Off);
  server.on("/led2on", led2On);
  server.on("/led2off", led2Off);

  server.begin();
}

void loop() {
  server.handleClient();
}