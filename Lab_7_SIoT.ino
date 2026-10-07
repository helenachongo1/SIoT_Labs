#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>
#include <ESP_Mail_Client.h>
#include "time.h"

// WIFI
const char* ssid = "Galaxy A25 5G E50E";
const char* password = "11111111";

// EMAIL
#define emailSenderAccount    "chongohelena905@gmail.com"
#define emailSenderPassword   "qgrhjnqcdcclhwly"
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "[ALERT] ESP32 Temperature"

// USER SETTINGS
String inputMessage = "helenitachongo@gmail.com";
String enableEmailChecked = "checked";
String inputMessage2 = "true";
String inputMessage3 = "25.0";

String lastTemperature = "--";
String lastHumidity = "--";

bool emailSent = false;

// DHT
#define DHTPIN 27
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// SERVER
AsyncWebServer server(80);
SMTPSession smtp;

// HTML
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
  <title>ESP32 Monitor</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>

<body>

<h2>Temperature</h2>
<h3>%TEMPERATURE% C</h3>

<h2>Humidity</h2>
<h3>%HUMIDITY% %</h3>

<hr>

<h2>Email Settings</h2>

<form action="/get">

<label>Email:</label><br>
<input type="email" name="email_input" value="%EMAIL_INPUT%" required><br><br>

<label>Enable Email:</label>
<input type="checkbox" name="enable_email_input" value="true" %ENABLE_EMAIL%><br><br>

<label>Threshold:</label><br>
<input type="number" step="0.1" name="threshold_input" value="%THRESHOLD%" required><br><br>

<input type="submit" value="Save">

</form>

</body>
</html>
)rawliteral";

// PROCESSOR
String processor(const String& var){
  if(var == "TEMPERATURE") return lastTemperature;
  if(var == "HUMIDITY") return lastHumidity;
  if(var == "EMAIL_INPUT") return inputMessage;
  if(var == "ENABLE_EMAIL") return enableEmailChecked;
  if(var == "THRESHOLD") return inputMessage3;
  return String();
}

// EMAIL FUNCTION
bool sendEmailNotification(String emailMessage) {

  ESP_Mail_Session session;

  session.server.host_name = smtpServer;
  session.server.port = smtpServerPort;

  session.login.email = emailSenderAccount;
  session.login.password = emailSenderPassword;
  session.login.user_domain = "";

  SMTP_Message message;

  message.sender.name = "ESP32";
  message.sender.email = emailSenderAccount;
  message.subject = emailSubject;

  message.addRecipient("User", inputMessage);

  message.text.content = emailMessage.c_str();
  message.text.charSet = "us-ascii";

  Serial.println("Connecting to SMTP...");

  if (!smtp.connect(&session)) {
    Serial.println("SMTP connection failed");
    Serial.println(smtp.errorReason());
    return false;
  }

  Serial.println("Sending email...");

  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("Error sending Email:");
    Serial.println(smtp.errorReason());
    smtp.closeSession();
    return false;
  }

  Serial.println("Email sent successfully!");
  smtp.closeSession();

  return true;
}

// SETUP
void setup() {
  Serial.begin(115200);

  dht.begin();  // ✅ IMPORTANT

  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());

  // TIME (required for email)
  configTime(0, 0, "pool.ntp.org");

  // WEB SERVER
 server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  AsyncWebServerResponse *response = request->beginResponse_P(
    200,
    "text/html",
    index_html,
    processor
  );
  response->addHeader("Cache-Control", "no-cache");
  request->send(response);
});

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {

    if (request->hasParam("email_input"))
      inputMessage = request->getParam("email_input")->value();

    if (request->hasParam("enable_email_input")) {
      inputMessage2 = "true";
      enableEmailChecked = "checked";
    } else {
      inputMessage2 = "false";
      enableEmailChecked = "";
    }

    if (request->hasParam("threshold_input"))
      inputMessage3 = request->getParam("threshold_input")->value();

    request->send(200, "text/html", "Saved! <a href=\"/\">Back</a>");
  });

  server.begin();
}

// LOOP
unsigned long previousMillis = 0;
const long interval = 5000;

void loop() {

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();

    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (isnan(temp) || isnan(hum)) {
      Serial.println("DHT failed!");
      return;
    }

    lastTemperature = String(temp);
    lastHumidity = String(hum);

    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.print(" | Hum: ");
    Serial.println(hum);

    if (temp > inputMessage3.toFloat() && inputMessage2 == "true" && !emailSent) {

      String msg = "Temperature HIGH: " + String(temp);

      if (sendEmailNotification(msg)) {
        Serial.println("Email sent!");
        emailSent = true;
      }
    }

    else if (temp < inputMessage3.toFloat() && inputMessage2 == "true" && emailSent) {

      String msg = "Temperature NORMAL: " + String(temp);

      if (sendEmailNotification(msg)) {
        Serial.println("Back to normal email sent!");
        emailSent = false;
      }
    }
  }
}

/*
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  delay(2000);
}

void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(t) || isnan(h)) {
    Serial.println("FAILED");
  } else {
    Serial.print("Temp: ");
    Serial.print(t);
    Serial.print(" | Hum: ");
    Serial.println(h);
  }

  delay(2000);
}*/