#ifndef WEBSOCKETS_H
#define WEBSOCKETS_H

#include <Arduino.h>
#include <AsyncWebSocket.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

extern Adafruit_NeoPixel pixels;

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

#endif