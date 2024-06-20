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



Measurement buffer[BUFFER_SIZE];
int bufferIndex = 0;

void captureMeasurements(){
  if (bufferIndex >= BUFFER_SIZE)
  {
    Serial.println("Measurement buffer full! Sending JSON");
    //sendMeasurements_JSON();
    sendMeasurements_Bin();
    bufferIndex = 0;
    return;
  }
  

  int val1 = analogRead(1);

  buffer[bufferIndex].current_Total = val1;
  buffer[bufferIndex].timestamp = millis();
  
  // Increment bufferIndex and wrap around if necessary
  bufferIndex++;
  return;
}


void sendMeasurements_Bin(){

  ws.binaryAll((uint8_t*)buffer, sizeof(buffer));
  return ;

}
/*
void sendMeasurements_JSON(){
    
  JsonDocument measurement;
      // Create a JSON document
    JsonDocument doc; // Adjust the size if necessary

    // Create a JSON array
    JsonArray array = doc.to<JsonArray>();

    // Populate the JSON array with buffer contents
    for (int i = 0; i < BUFFER_SIZE; i++) {
        JsonObject obj = array.add<JsonObject>();
        obj["IT"] = buffer[i].current_Total;
        obj["IL"] = buffer[i].current_L;
        obj["IR"] = buffer[i].current_R;
        obj["s"] = buffer[i].setpoint;
        obj["e"] = buffer[i].error;
        obj["eL"] = buffer[i].encoder_L;
        obj["eR"] = buffer[i].encoder_R;
        obj["t"] = buffer[i].timestamp;
    }

  char output[4096] = {};
  serializeJson(doc, output);

  ws.textAll(output);
  return ;
}*/