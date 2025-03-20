#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char *SSID = "PLC";
const char *PWD = "12345678";

int views;

// LCD setup: I2C address is 0x27, 16x2 dimensions
LiquidCrystal_I2C lcd(0x27, 16, 2);

// The WebServer object will be used to set up the ESP32 web server
WebServer server(80);

StaticJsonDocument<250> jsonDocument;

void setup_routing() {            
  server.on("/views", HTTP_POST, handleViews);
  server.on("/views", HTTP_OPTIONS, handleOptions);  
  server.begin();    
}

void handleOptions() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.send(200, "application/json", "{OK}");
}

void handleViews() {
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  //{
  //  views: 55,
  //}
  views = jsonDocument["views"];
  
  // Serial output
  Serial.println(views);
  
  // LCD output
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Views:");
  lcd.setCursor(0, 1);
  lcd.print(views);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", "{OK}");
}

void setup() {
  // Initialize Serial
  Serial.begin(9600);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.print("Initializing");

  // Wi-Fi connection
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(SSID, PWD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    lcd.setCursor(0, 1);
    lcd.print("Connecting...");
    delay(500);
  }

  // Display connected status
  Serial.print("Connected! IP Address: ");
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.print("Connected!");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP().toString());

  setup_routing();     
}

void loop() {
  server.handleClient();   
}

