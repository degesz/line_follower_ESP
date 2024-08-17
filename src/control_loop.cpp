#include "control_loop.h"

double Setpoint = 16, Input, Output;

double Kp = 0, Ki = 0, Kd = 0;

PID pid(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);


void setup_controlLoop(){
  pinMode(parallelLoadPin, OUTPUT);
  pinMode(shiftClkPin, OUTPUT);
  pinMode(shiftDataPin, INPUT);

    //apply PID gains
  pid.SetTunings(Kp, Ki, Kd);


  //turn the PID on
  pid.SetMode(AUTOMATIC);
}




void controlLoop(){


//shift in bits from sensor
bool sensorBits[32] = {0};

digitalWrite(parallelLoadPin, HIGH);    //Load parallel data into shift registers
// GPIO.out_w1ts = (1 << parallelloadPin);


for (byte i = 0; i < 32; i++) // Shift in all 32 bits
{
  digitalWrite(shiftClkPin, HIGH);    // Pulse clock signal to advance shift registers
  // GPIO.out_w1ts = (1 << shiftClkPin);

  sensorBits[i] = digitalRead(shiftDataPin);  // Read the bit into the array
  // sensorBits[i] = (GPIO.in >> 4) & 0x1;

  digitalWrite(shiftClkPin, LOW); 
  // GPIO.out_w1tc = (1 << shiftClkPin);

}

digitalWrite(parallelLoadPin, LOW);
// GPIO.out_w1tc = (1 << parallelloadPin);

///////////PROCESSING
byte leftBit = 0;     // leftmost and rightmost active bits
byte rightBit = 0;

for (byte i = 1; i < 31; i++)   // find the bits, to reject false positives, two consecutive bits must be active
{
  if (sensorBits[i] == 1 && sensorBits[i+1] == 1)
  {
    leftBit = i;
    break;
  }
}

for (byte i = 31; i > 1; i--)
{
  if (sensorBits[i] == 1 && sensorBits[i-1] == 1)
  {
    rightBit = i;
    break;
  }
}

Input = (leftBit + rightBit) / 2.0 ;   // compute the average, which sshould be middle of line


//process bits to input value

pid.Compute();

//output to motors

}



