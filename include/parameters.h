#ifndef PARAMS_H
#define PARAMS_H

#include <Arduino.h>
#include <Preferences.h>

struct params_t {
    int16_t P0;
    int16_t P1;
    int16_t P2;
    int16_t P3;
    int16_t P4;
    
};


void paramsBegin();
void updateParams(params_t params);
params_t readParams();




#endif