#include "WiFi.h"
#include "ESPAsyncWebSrv.h"
#include <HTTPClient.h>

// Set your access point network credentials
const char* ssidServer = "RelayWiFi";
const char* passwordServer = "123456789";
const char* ssidClient = "ChildWiFi";
const char* passwordClient = "123456789";

AsyncWebServer server(80);

String pedometer = "0";

//Your IP address or domain name with URL path
const char* serverNamePedometer = "http://192.168.4.1/pedometer";

bool clientMode = 0;

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
  
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) {
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

void setup(){
  Serial.begin(115200);
  Serial.println();
}

void loop(){
  
  if(clientMode) {
    WiFi.begin(ssidClient, passwordClient);

    Serial.println("Connecting");

    while(WiFi.status() != WL_CONNECTED) { 
      delay(250);
      Serial.print(".");
    }

    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
    
      // Check WiFi connection status
      if(WiFi.status()== WL_CONNECTED ){ 
        pedometer = httpGETRequest(serverNamePedometer);
        Serial.println("Pedometer: " + pedometer);
      }
      else {
        Serial.println("WiFi Disconnected");
      }

  }
  else {
    Serial.print("Setting AP (Access Point)…");
    WiFi.softAP(ssidServer, passwordServer);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    server.on("/pedometer", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/plain", pedometer.c_str());
    });
    server.begin();
    delay(5000);
  } 
  clientMode = !clientMode;
}
