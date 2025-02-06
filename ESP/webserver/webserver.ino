#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

const char *SSID = "PLC";
const char *PWD = "12345678";

int views;

//The WebServer object will be used to set up the ESP32 web server. We will pass the default HTTP port which is 80, as the input to the constructor. This will be the port where the server will listen to the requests.
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
  Serial.println(views); 
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", "{OK}");
}


void setup() {  
  Serial.begin(9600); 
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(SSID, PWD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
 
  Serial.print("Connected! IP Address: ");
  Serial.println(WiFi.localIP());
  
  setup_routing();     
   
}    
       
void loop() {
  server.handleClient();   
}