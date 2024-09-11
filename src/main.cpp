#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include <Ticker.h>
#include <AS5600.h>
#include <PID_v1.h>
#include <Preferences.h>


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");


AS5600 encoder;

#include "parameters.h"
#include "websockets.h"
#include "encoders.h"
#include "credentials.h"
#include "control_loop.h"
#include "RGBleds.h"
#include "motor_control.h"



Ticker measurement_timer(captureMeasurements, 1, 0, MILLIS);
Ticker encoder_timer(updateEncoders, 100, 0, MILLIS);
Ticker control_loop_timer(controlLoop, 10, 0, MILLIS);
Ticker LEDs_timer(LEDs_update, 40, 0, MILLIS);

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
    Serial.begin(921600);
    Serial.setDebugOutput(true);  // sends all log_e(), log_i() messages to USB HW CDC
    Serial.setTxTimeoutMs(0);       // sets no timeout when trying to write to USB HW CDC
  //  delay(4000);
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

    paramsBegin();


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

      ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else {  // U_SPIFFS
            type = "filesystem";
        }
        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
    });
    
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
            Serial.println("OTA Auth Failed");
        } else if (error == OTA_BEGIN_ERROR) {
            Serial.println("OTA Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
            Serial.println("OTA Connect Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
            Serial.println("OTA Receive Failed");
        } else if (error == OTA_END_ERROR) {
            Serial.println("OTA End Failed");
        }
    });

    ArduinoOTA.begin();


  // Start server
  server.begin();
  setup_controlLoop();
  LEDs_setup();
  motor_setup();

  initEncoders();


  encoder_timer.start();
  control_loop_timer.start();
  measurement_timer.start();
  LEDs_timer.start();
}

void loop(){

  encoder_timer.update();
  control_loop_timer.update();
  measurement_timer.update();
  LEDs_timer.update();

  ws.cleanupClients();
  ArduinoOTA.handle();
}