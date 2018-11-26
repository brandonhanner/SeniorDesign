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
  uint32_t last_round = millis();
  uint32_t delta_t = 0;
  bool not_done = true;
  int i = 0;
  float time0 = 0.0;
  float time1 = 0.0;
  static uint32_t msec_prev = 0;
  float max = 0.0;
  float min = 100.0;
  float maxdelt = 0.0;
  float mindelt = 100.0;

  while (i<10000)
  {
    uint32_t msec_curr = millis();

    if (msec_curr-msec_prev > 8) {
	start = millis();
        delta_t = start - last_round;
        i++;
        imu.updateIMU();
        time0+= (float) delta_t;
        msec_prev = msec_curr;
        curVal = (weight*imu.getXaccel()) + ((1-weight) * prevVal);
        prevVal = curVal;
	end = millis();
        prevTime += end-start;
        if (end - start > max) max = end-start;
        if (end - start < min) min = end-start;
        if (delta_t > maxdelt) maxdelt = (float) delta_t;
	if (delta_t < mindelt) mindelt = (float) delta_t;
        printf(" %f -- %d -- %d\n",curVal, end-start ,delta_t);
        last_round = millis();
        //yn = w × xn + (1 – w) × yn – 1
    };
  }
  printf("%f , %f, %f\n", time0/10000.0, ((float)prevTime)/10000.0, (time0/10000.0)+(((float)prevTime)/10000.0));
  printf("max loop: %f, min loop: %f, max delt: %f, min delt: %f\n", max, min, maxdelt, mindelt);
}
