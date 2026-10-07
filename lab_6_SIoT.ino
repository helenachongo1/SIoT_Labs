#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

// Replace with your network credentials
const char* ssid = "Galaxy A25 5G E50E";
const char* password = "11111111";

// mDNS name (customize this as needed)
const char* mdnsName = "Me_005_esp32";

// Create a web server object
WebServer server(80);

// Function to handle the root URL
void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<link rel=\"icon\" href=\"data:,\">";
  html += "<style>";
  html += "body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif; background: white; color: #333; margin: 20px; }";
  html += "h1 { color: #4a5568; font-weight: 300; }";
  html += "</style></head>";
  html += "<body>";
  html += "<h1>Hello World</h1>";
  html += "<p>ESP32 mDNS Demo.</p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Set mDNS
  if (!MDNS.begin(mdnsName)) {
    Serial.println("Error setting up MDNS responder!");
    while(1) {
      delay(1000);
    }
  }

  // Add service to MDNS-SD
  MDNS.addService("_http", "_tcp", 80);
  Serial.println("mDNS responder started. Access your ESP32 at http://" + String(mdnsName) + ".local");


  // Set up the web server to handle route
  server.on("/", handleRoot);

  // Start the web server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Handle incoming client requests
  server.handleClient();
}