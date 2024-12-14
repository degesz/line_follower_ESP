#ifndef RGBLEDS_H
#define RGBLEDS_H

const int ledPin = 3;
const int numPixels = 10;

const int Led_Sig1_pin = 5;
const int Led_Sig2_pin = 6;

#include <Adafruit_NeoPixel.h>



void LEDs_setup();
void LEDs_update();

#endif