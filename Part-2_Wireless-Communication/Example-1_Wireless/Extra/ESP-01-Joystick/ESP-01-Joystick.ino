#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"
#include "FS.h"

// Default SSID and password
//const char ssid = "ESP8266";
//const char password = "12341234";
String ssid = "ESP8266";
String pass = "12341234";
String ip = "192.168.0.1";


AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

int xPos = 0;
int yPos = 0;
bool newData = false;

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len){
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len){
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    String joystickStr = String((char*)data);
    // Find X:
    int xIdx = joystickStr.indexOf("x:");
    // Find y:
    int yIdx = joystickStr.indexOf("y:");
    if (xIdx != -1 && yIdx != -1){
      String xStr = joystickStr.substring(xIdx+2, yIdx);
      xPos = xStr.toInt();
      String yStr = joystickStr.substring(yIdx+2);
      yPos = yStr.toInt();
      yPos = yPos;
      newData = true;
      notifyClients();
    }
  }
}

void notifyClients() {
  String notification = String("Received X: " + String(xPos, DEC) + " and Y: " + String(yPos, DEC));
  ws.textAll(notification);
}

void initWebSocket(){
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void setup(){
  // Begin Serial:
  Serial.begin(9600);
  
  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Await startup settings:
  const int waitTime = 5000; //ms 
  const int waitRes = 50; //ms
  int delayCnt = 0;
  String serialIn = ""; 
  const String ipKey = "ip:";
  const String ssidKey = "ssid:";
  const String passKey = "pass:";
  int ipIdx = 0;
  int ipTerm = 0;
  int ssidIdx = 0;
  int ssidTerm = 0;
  int passIdx = 0;
  int passTerm = 0;
  
  while(1){
    if (Serial.available() > 0){
      serialIn.concat(Serial.readString());
    }

    ipIdx = serialIn.indexOf(ipKey);
    ipTerm = serialIn.indexOf("\n", ipIdx + ipKey.length());
    ssidIdx = serialIn.indexOf(ssidKey);
    ssidTerm = serialIn.indexOf("\n", ssidIdx + ssidKey.length());
    passIdx = serialIn.indexOf(passKey);
    passTerm = serialIn.indexOf("\n", passIdx + passKey.length());
    
    if(!(ipIdx == -1) && !(ipTerm == -1) &&
        !(ssidIdx == -1) && !(ssidTerm == -1) &&
        !(passIdx == -1) && !(passTerm == -1)){
      Serial.println();
      Serial.println();
      Serial.println(ipIdx);
      Serial.println(ssidIdx);
      Serial.println(passIdx);
      Serial.println();
      ipIdx += ipKey.length();
      ssidIdx += ssidKey.length();
      passIdx += passKey.length();
      Serial.println(serialIn);
      ip = serialIn.substring(ipIdx, ipTerm);
      ssid = serialIn.substring(ssidIdx, ssidTerm);
      pass = serialIn.substring(passIdx, passTerm);
      break;
    }
    
    delay(waitRes);
    delayCnt++;
    if ((delayCnt * waitRes) >= waitTime){
      break;
    }
  }
  ip.trim();
  ssid.trim();
  pass.trim();

  IPAddress apIP;
  apIP.fromString(ip.c_str());
  
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid.c_str(), pass.c_str());
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Serial.print("Password: ");
  Serial.println(myIP);
  
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false);
  });

  // Initialize Websocket
  initWebSocket();

  // Start server
  server.begin();
}

void loop(){
  ws.cleanupClients();
  if (newData == true){
    String notification = String("X:" + String(xPos, DEC) + "Y:" + String(yPos, DEC));
    Serial.println(notification);
    newData = false;
  }
}
