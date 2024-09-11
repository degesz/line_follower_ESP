#ifndef WEBSOCKETS_H
#define WEBSOCKETS_H

#include <Arduino.h>
#include <AsyncWebSocket.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include "motor_control.h"
#include "control_loop.h"
#include "parameters.h"

extern Adafruit_NeoPixel pixels;
extern AsyncWebSocket ws;

extern int32_t cumulative_R;
extern int32_t cumulative_L;


extern void updateParams(params_t params);
extern params_t readParams();
extern double Setpoint, Input, Output;

extern int operationMode;
extern int manual_Joy_X, manual_Joy_Y;

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void sendMeasurements_JSON();
void captureMeasurements();
void slowMeas();
void sendMeasurements_Bin();


struct __attribute__((packed)) Measurement {
    int16_t current_L;
    int16_t current_R;
    int16_t current_Total;
    int16_t setpoint;
    int16_t input;
    uint32_t encoder_L;
    uint32_t encoder_R;
    uint32_t timestamp; // Store the time when the measurement was taken
    uint16_t output;
};

const int BUFFER_SIZE = 100;


#endif