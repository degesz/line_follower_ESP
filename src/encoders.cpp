#include "encoders.h"

int32_t cumulative_R = 0;
int32_t cumulative_L = 0;
    int prevAngle_L = 0;
    int prevAngle_R = 0;


void updateEncoders(){
    int diff = 0;
    int currentAngle = 0;

    digitalWrite(ENCODER_SWITCH_PIN, LOW);
    currentAngle = encoder.readAngle();
    diff = prevAngle_L - currentAngle ;
    if (diff < -3000  || diff > 3000 )
    {
        diff *= -1;
        if (diff < 0)
        {
            diff += 4096;
        }
        else
        {
            diff -= 4096;
        }
    }
    cumulative_L += diff;
    prevAngle_L = currentAngle;

    digitalWrite(ENCODER_SWITCH_PIN, HIGH);
    currentAngle = encoder.readAngle();
    diff = prevAngle_R - currentAngle ;
    if (diff < -3000  || diff > 3000 )
    {
        diff *= -1;
        if (diff < 0)
        {
            diff += 4096;
        }
        else
        {
            diff -= 4096;
        }
    }
    cumulative_R -= diff;
    prevAngle_R = currentAngle;

    Serial.printf("   Enc: L %d    R %d \n", cumulative_L, cumulative_R);
    return;



}
void initEncoders(){
  Wire.begin();

    pinMode(ENCODER_SWITCH_PIN, OUTPUT);

    digitalWrite(ENCODER_SWITCH_PIN, HIGH);

  encoder.begin();

  if (encoder.isConnected())
  {
    Serial.print("Encoder 1 connected ");
  }
    digitalWrite(ENCODER_SWITCH_PIN, LOW);
 // encoder.begin();
  if (encoder.isConnected())
  {
    Serial.print("Encoder 2 connected ");
  }
  Serial.println();
  return;
}