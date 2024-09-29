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
      client->printf("{\"type\":\"params\",\"P0\":%d,\"P1\":%d,\"P2\":%d,\"P3\":%d,\"P4\":%d}", loadedParams.P0, loadedParams.P1, loadedParams.P2, loadedParams.P3, loadedParams.P4);
    }
    else if(msg["type"] == "config-update")
    {
      Serial.println("NEW CONFIG");
      Serial.printf("Received: %s\n", message.c_str());
      params_t newParams = {(int)(msg["P0"].as<float>() * 10),(int)(msg["P1"].as<float>() * 10),(int)(msg["P2"].as<float>() * 10),(int)(msg["P3"].as<float>() * 10),(int)(msg["P4"].as<float>())};
      updateParams(newParams);
      if (newParams.P4 != 0)
      {
        motors_set_pwm_frequency(newParams.P4); // update the motor frequency
      }
      
      
    }
    else if (msg["type"] == "AUTO_CONTROL")
    {
      operationMode = 0;
      Serial.println("Auto operation mode 0 set");
    }
    else if (msg["type"] == "MANUAL_CONTROL")
    {
      operationMode = 1;
      Serial.println("Manual operation mode 1 set");
    }
    else if (msg["type"] == "CONTROL_DATA")
    {
      manual_Joy_X = msg["joyX"] ;
      manual_Joy_Y = msg["joyY"] ;
      float manualSpeed =  msg["manualSpeed"];
      manual_Joy_X *= (manualSpeed / 10);
      manual_Joy_Y *= (manualSpeed / 10);
    }
    else if (msg["type"] == "CAL-LINE")
    {
      /* code */
      Serial.println("Line calibration command");
    }
    else if (msg["type"] == "CAL-TURN")
    {
      /* code */
      Serial.println("Turn calibration command");
    }
    else if (msg["type"] == "RESET")
    {
      Serial.println("Resetting on command...");
      ESP.restart();
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

  int curr_L = analogRead(13);
  int curr_R = analogRead(14);

  buffer[bufferIndex].current_L = curr_L;
  buffer[bufferIndex].current_R = curr_R;

  buffer[bufferIndex].current_Total = curr_L + curr_R;    // total current sensor not implemented :(

  buffer[bufferIndex].setpoint = (int)Setpoint * 10;
  buffer[bufferIndex].input = (int)Input * 10;
  buffer[bufferIndex].output = (int)( map(Output * 100.0, -1600, 1600, 0, 3200) );

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



