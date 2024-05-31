#include <Arduino.h>


#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>

#define PIN        18
#define NUMPIXELS 1

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const char* ssid = "Zajebiste hotspot";
const char* password = "12345678";

unsigned long previousMillis = 0;
const long interval = 500; // 10 seconds

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// Handle 404 errors
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

// WebSocket event handler
void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.println("WebSocket client connected");
    client->text("Welcome to the WebSocket server!");
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.println("WebSocket client disconnected");
  } else if (type == WS_EVT_DATA) {
    Serial.println("WebSocket data received");

    // Convert data to a String
    String msg = "";
    for (size_t i = 0; i < len; i++) {
      msg += (char) data[i];
    }

    // Print the received message
    Serial.printf("Received: %s\n", msg.c_str());

   // Parse the JSON data
   StaticJsonDocument<200> doc;
   DeserializationError error = deserializeJson(doc, msg);

   if (error) {
     Serial.print(F("deserializeJson() failed: "));
     Serial.println(error.f_str());
     return;
   }

   // Extract values from the JSON object
   //byte sliderColor = doc["color"];
   int red = doc["red"];
   int green = doc["green"];
   int blue = doc["blue"];

  pixels.setPixelColor(0, pixels.Color(red, green, blue));
    pixels.show(); 
  }
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

  // Handle 404
  server.onNotFound(notFound);

  // WebSocket event handler
  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);

  // Start server
  server.begin();

  pixels.begin();
}

void loop(){
    unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ws.textAll("Hello from ESP32");
  }

  //  pixels.setPixelColor(0, pixels.Color(0, 150, 0));

    pixels.show(); 

  ws.cleanupClients();
}

