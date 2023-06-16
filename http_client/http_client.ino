#include <WiFi.h>
#include <HTTPClient.h>
#include <SoftwareSerial.h>
#include <XBee.h>

SoftwareSerial xbeeSerial(16, 17);  // RX (D2), TX (D3) - Adjust the pin numbers as per your wiring
XBee xbee = XBee();
XBeeAddress64 destAddr = XBeeAddress64(0x0000000000882235);  // Destination address

const char* ssidServer = "ChildWiFi";
const char* passwordServer = "123456789";
const char* ssidRelay = "RelayWiFi";
const char* passwordRelay = "123456789";

//Your IP address or domain name with URL path
const char* serverNamePedometer = "http://192.168.4.1/pedometer";

String pedometer;

unsigned long previousMillis = 0;
const long interval = 5000; 

void setup() {
  Serial.begin(9600);
  xbeeSerial.begin(9600);
  xbee.setSerial(xbeeSerial);
}

void loop() {
  int status = WiFi.status();
  unsigned long currentMillis = millis();

  //Trying to connect to the captor ssid
  if(status != WL_CONNECTED) {
    WiFi.begin(ssidServer, passwordServer);
    Serial.println("Connecting to Captor WiFi network.");
    int tries = 10; int i = 0;
    while(status != WL_CONNECTED && i < tries) { 
      delay(500);
      Serial.print(".");
      i++;
      status = WiFi.status();
    }

    if(status == WL_CONNECTED)
    {
      Serial.println("");
      Serial.print("Connected to Captor WiFi network with IP Address: ");
      Serial.println(WiFi.localIP());
    }
    else
    {
      WiFi.begin(ssidRelay, passwordRelay);
      Serial.println("Connecting to Relay WiFi network.");
      int i = 0; status = WiFi.status();
      while(status != WL_CONNECTED && i < tries) { 
        delay(500);
        Serial.print(".");
        i++;
        status = WiFi.status();
      }
      if(status == WL_CONNECTED)
      {
        Serial.println("");
        Serial.print("Connected to Relay WiFi network with IP Address: ");
        Serial.println(WiFi.localIP());
      }
      else return;
    }
  }

  if(currentMillis - previousMillis >= interval) {
     // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED ){ 
      pedometer = httpGETRequest(serverNamePedometer);
      Serial.println("Pedometer: " + pedometer);
      
      // save the last HTTP GET Request
      previousMillis = currentMillis;
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  }
  
  //Envoi d'un message
  char message[] = "Very funny from Antoto!\n";
  xbeeSerial.write(message);

  //réception d'un message
  xbeeSerial.write("+++");

  if (xbeeSerial.available()) {
    Serial.println("Available.");
    // Read all available bytes
    while (xbeeSerial.available()) {
      char incomingByte = xbeeSerial.read();
      // Do something with the received byte
      Serial.print("Received: ");
      Serial.println(incomingByte);
    }
  }
  
}

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
