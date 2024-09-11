#ifndef CTRL_LOOP_H
#define CTRL_LOOP_H

#include <Arduino.h>
#include <PID_v1.h>
#include "parameters.h"
#include "motor_control.h"


const int parallelLoadPin = GPIO_NUM_1;
const int shiftClkPin = GPIO_NUM_2;
const int shiftDataPin = GPIO_NUM_4;

void setup_controlLoop();
void controlLoop();



extern params_t savedParams;

#endif