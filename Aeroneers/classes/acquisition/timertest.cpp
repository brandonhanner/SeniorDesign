#include"IMU.h"
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdint.h>
int main()
{
  myIMU imu;
  float prevVal=0.0;
  float curVal = 0.0;
  float weight = 0.2;
  uint32_t start = 0;
  uint32_t end = 0;
  uint32_t prevTime = 0;
  uint32_t last_round = 0;
  uint32_t delta_t = 0;
  bool not_done = true;
  int i = 0;
  float time0 = 0.0;
  float time1 = 0.0;

  while (i<1000)
  {
    start = millis();
    imu.updateIMU();
    time0 += millis()-start;
    i++;
    /*
    test comment
    static uint32_t msec_prev;
    uint32_t msec_curr = millis();

    if (msec_curr-msec_prev > 11) {
        i++;
        delta_t = millis()-last_round;
        //imu.updateIMU();
        time0+= (float) delta_t;
        msec_prev = msec_curr;
        //curVal = (weight*imu.getXaccel()) + ((1-weight) * prevVal);
        prevVal = curVal;
        //printf(" %f -- %d -- %d\n",curVal, prevTime,delta_t);
        last_round = millis();
        //yn = w × xn + (1 – w) × yn – 1

    }*/
    //end = millis();
    //prevTime = end-start;
  }
  printf("%f\n", time0/1000.0);
}
