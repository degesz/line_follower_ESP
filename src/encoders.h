#ifndef ENC_H
#define ENC_H

#include <AS5600.h>
#include <Arduino.h>


#define ENCODER_SWITCH_PIN 17

extern AS5600 encoder;



void updateEncoders();
void initEncoders();




#endif //ENC_H