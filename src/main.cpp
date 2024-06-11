#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include <Ticker.h>



#define PIN 18
#define NUMPIXELS 1

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

unsigned long previousMillis = 0;
const long interval = 500; // 10 seconds

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");



#include "websockets.h"
#include "credentials.h"

Ticker measurement_timer(sendMeasurements, 100, 0, MILLIS);


// Handle 404 errors
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String get_wifi_status(int status){
    switch(status){
        case WL_IDLE_STATUS:
        return "WL_IDLE_STATUS";
        case WL_SCAN_COMPLETED:
        return "WL_SCAN_COMPLETED";
        case WL_NO_SSID_AVAIL:
        return "WL_NO_SSID_AVAIL";
        case WL_CONNECT_FAILED:
        return "WL_CONNECT_FAILED";
        case WL_CONNECTION_LOST:
        return "WL_CONNECTION_LOST";
        case WL_CONNECTED:
        return "WL_CONNECTED";
        case WL_DISCONNECTED:
        return "WL_DISCONNECTED";

        default:
          return "";
    }
}

void setup(){
    Serial.begin(115200);
    delay(10);
    int status = WL_IDLE_STATUS;
    Serial.println("\nConnecting");
   // Serial.println(get_wifi_status(status));
    WiFi.begin(ssid, password);
    while(status != WL_CONNECTED){
        delay(500);
        status = WiFi.status();
    //    Serial.println(get_wifi_status(status));
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());


     //Initialize SPIFFS
    if (!SPIFFS.begin(true)) {
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }

  // Serve the HTML file
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  // Serve the JavaScript file
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/script.js", "application/javascript");
  });

    // Wildcard handler for any file
  server.onNotFound([](AsyncWebServerRequest *request) {
    String filename = request->url();
    String contentType = "text/plain"; // Default to text/plain
    if (filename.endsWith(".js")) {
      contentType = "application/javascript";
    } else if (filename.endsWith(".css")) {
      contentType = "text/css";
    } else if (filename.endsWith(".html")) {
      contentType = "text/html";
    }

    if (SPIFFS.exists(filename)) {
      request->send(SPIFFS, filename, contentType);
    } else {
      request->send(404, "text/plain", "File not found");
    }
  });

  // WebSocket event handler
  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);

  // Start server
  server.begin();

  pixels.begin();


  measurement_timer.start();
}

void loop(){

  measurement_timer.update();

  ws.cleanupClients();
}