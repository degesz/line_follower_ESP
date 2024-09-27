#include "control_loop.h"

double Setpoint = 16, Input, Output;
double Kp = 1, Ki = 0, Kd = 0;
double speed;

PID pid(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

int operationMode = 0; // 0 = PID line follow      1 = Manual web interface control

int manual_Joy_X, manual_Joy_Y = 0;


void setup_controlLoop(){
  pinMode(parallelLoadPin, OUTPUT);
  pinMode(shiftClkPin, OUTPUT);
  pinMode(shiftDataPin, INPUT);
    
  pid.SetTunings(Kp, Ki, Kd);//apply PID gains
  pid.SetOutputLimits(-16, 16);
  pid.SetMode(AUTOMATIC);//turn the PID on
}

void updateTunings(){
  pid.SetTunings(Kp, Ki, Kd);//apply PID gains
  Serial.printf("new tunings: P: %2f  I: %2f  D: %2f \n", Kp, Ki, Kd);
}

void controlLoop(){

if (operationMode == 0)  /////////////  Auto PID mode 0
{
  //////shift in bits from sensor
bool sensorBits[32] = {0};
 //Load parallel data into shift registers
GPIO.out_w1ts = (1 << parallelLoadPin);


for (byte i = 0; i < 32; i++) // Shift in all 32 bits
{
 // Read the bit into the array
   sensorBits[i] = (GPIO.in >> 4) & 0x1;
   sensorBits[i] = !sensorBits[i];

  // Pulse clock signal to advance shift registers
   GPIO.out_w1ts = (1 << shiftClkPin);
   GPIO.out_w1tc = (1 << shiftClkPin);

}

 GPIO.out_w1tc = (1 << parallelLoadPin);


//for (int i = 0; i < 32; i++) {
//    Serial.print(sensorBits[i] ? "■" : " ");
//  }
//Serial.println();  // Final newline after all bits are printed

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


//Serial.printf("In: %f   Out: %f\n", Input, Output);

int motor_L = constrain(50 - speed * 5 + Output, 0, 100);
int motor_R = constrain(50 - speed * 5 - Output, 0, 100);
Serial.printf("Speed: %1f    Output: %2f    L: %d   R: %d   \n",speed, Output, motor_L, motor_R); 
motor_write(motor_L, motor_R);

}

else if (operationMode == 1)  // Manual controle mode 1 
{
  

  int baseSpeed = map(manual_Joy_Y, -100, 100, 50, -50);
  int turnEffect = map(manual_Joy_X, -100, 100, 45, -45);

  int motor_L = constrain(50 + baseSpeed + turnEffect, 0, 100);
  int motor_R = constrain(50 + baseSpeed - turnEffect, 0, 100);

  //Serial.printf("bSp: %d    turE: %d    L: %d   R: %d   \n",baseSpeed, turnEffect, motor_L, motor_R); 
  motor_write(motor_L, motor_R);


}




}



