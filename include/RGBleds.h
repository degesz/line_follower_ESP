#ifndef RGBLEDS_H
#define RGBLEDS_H

const int ledPin = 35;
const int numPixels = 10;

#include <Adafruit_NeoPixel.h>



void LEDs_setup();
void LEDs_update();
void rainbowCycle();
uint32_t Wheel(byte WheelPos);

#endif