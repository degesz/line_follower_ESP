#include "RGBleds.h"

uint8_t brightness = 100;

Adafruit_NeoPixel pixels(numPixels, ledPin, NEO_GRB + NEO_KHZ800);

void LEDs_setup() {
  pinMode(ledPin, OUTPUT);
  pixels.begin();
  pixels.setBrightness(brightness); // Set brightness level
}

long hue = 0;  

int fadeStep = 15;
const int fadeTime = 50;
uint32_t lastFadeTime = 0;
int Sig_Led_2_PWM = 0;


void LEDs_update(){

  if (millis() > lastFadeTime + fadeTime)
  {
    lastFadeTime = millis();
    Sig_Led_2_PWM += fadeStep;
    constrain(Sig_Led_2_PWM, 0, 255);
    if (Sig_Led_2_PWM == 0 || Sig_Led_2_PWM == 255)
    {
      fadeStep *= -1;
    }
    analogWrite(Led_Sig2_pin, Sig_Led_2_PWM);
    
  }
  

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
