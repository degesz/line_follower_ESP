#include "websockets.h"

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.println("WebSocket client connected");
    client->text("Connected!");
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.println("WebSocket client disconnected");
  } else if (type == WS_EVT_DATA) {
    Serial.println("WebSocket data received");

    // Convert data to a String
    String message = "";
    for (size_t i = 0; i < len; i++) {
      message += (char) data[i];
    }

    // Print the received message
    Serial.printf("Received: %s\n", message.c_str());

    // Parse the JSON data
    JsonDocument msg;
    DeserializationError error = deserializeJson(msg, message);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    if (msg["type"] == "ping") {
      client->text(message);    // return ping message to client
    }
  }
}


void sendMeasurements(){
    int val = analogRead(1);
  JsonDocument measurement;
  measurement["type"] = "measurement";
  measurement["val1"] = val;
  char output[100] = {};
  serializeJson(measurement, output);

  ws.textAll(output);
}