#include "websockets.h"



void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.println("WebSocket client connected");
    client->text("Connected!");
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.println("WebSocket client disconnected");
  } else if (type == WS_EVT_DATA) {

    // Convert data to a String
    String message = "";
    for (size_t i = 0; i < len; i++) {
      message += (char) data[i];
    }

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
    else if(msg["type"] == "config-request")
    {
      Serial.println("CONFIG REQUEST");
      params_t loadedParams = readParams();
      client->printf("{\"type\":\"params\",\"P0\":%d,\"P1\":%d,\"P2\":%d,\"P3\":%d}", loadedParams.P0, loadedParams.P1, loadedParams.P2, loadedParams.P3);
    }
    else if(msg["type"] == "config-update")
    {
      Serial.println("NEW CONFIG");
      Serial.printf("Received: %s\n", message.c_str());
      params_t newParams = {(int)(msg["P0"].as<float>() * 10),(int)(msg["P1"].as<float>() * 10),(int)(msg["P2"].as<float>() * 10),(int)(msg["P3"].as<float>() * 10)};
      updateParams(newParams);

    }
    else
    {
      Serial.printf("Received: %s\n", message.c_str());
    }
    
    
  }
}



Measurement buffer[BUFFER_SIZE];
int bufferIndex = 0;

void captureMeasurements(){
  if (bufferIndex >= BUFFER_SIZE)
  {
    sendMeasurements_Bin();
    bufferIndex = 0;
    return;
  }
  
  //Serial.print(mag_1.readAngle());

  int val1 = analogRead(1);

  buffer[bufferIndex].current_Total = val1;
  buffer[bufferIndex].encoder_L = cumulative_L;
  buffer[bufferIndex].encoder_R = cumulative_R;
  buffer[bufferIndex].timestamp = millis();
  
  // Increment bufferIndex and wrap around if necessary
  bufferIndex++;
  return;
}


void sendMeasurements_Bin(){
//  Serial.print("sending----");
  ws.binaryAll((uint8_t*)buffer, sizeof(buffer));
//  Serial.println("sent");
  return ;

}



