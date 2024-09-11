#include "RGBleds.h"

uint8_t brightness = 18;

Adafruit_NeoPixel pixels(numPixels, ledPin, NEO_GRB + NEO_KHZ800);

void LEDs_setup() {
  pinMode(ledPin, OUTPUT);
  pixels.begin();
  pixels.setBrightness(brightness); // Set brightness level
}

long hue = 0;  

void LEDs_update(){

    for (int i = 0; i < numPixels; i++) {
      int pixelHue = (hue + (i * 65536L / numPixels)) % 65536;
      pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
    }

    pixels.show(); // Display the updated pixels

    hue += 1200; // Increment the hue for the next iteration
    if (hue >= 65536) {
      hue = 0;  // Wrap around when hue reaches maximum
    }

}
