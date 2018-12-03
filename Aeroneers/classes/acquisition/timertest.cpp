#include"IMU.h"
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdint.h>
int main()
{
  myIMU imu;
  imu.calibrateAccelerometer();
  float prevVal=0.0;
  float curVal = 0.0;
  float weight = 0.2;
  uint32_t start = 0;
  uint32_t end = 0;
  uint32_t prevTime = 0;
  uint32_t last_round = millis();
  uint32_t trip = 0;
  int i = 0;
  float time0 = 0.0;
  static uint32_t msec_prev = millis();
  float max = 0.0;
  float min = 100.0;
  float maxdelt = 0.0;
  float mindelt = 100.0;
  int numberOfIterations = 1500;
  static uint32_t msec_curr = 0;

  while (i<numberOfIterations)
  {
    msec_curr = millis();

    if ((msec_curr-msec_prev) > 8) 
    {
	start = millis();
        trip = start - last_round;
        i++;
        imu.updateAccel();
        time0+= (float) trip;
        msec_prev = msec_curr;
        curVal = (weight*imu.getXaccel()) + ((1-weight) * prevVal);
        prevVal = curVal;
	end = millis();
        prevTime += end-start;
        if (end - start > max) max = end-start;
        if (end - start < min) min = end-start;
        if (trip > maxdelt) maxdelt = (float) trip;
	if (trip < mindelt) mindelt = (float) trip;
        printf(" Accel: %+-8f \t loop: %-8d \t delta: %-8d\n",curVal, end-start ,trip);
        last_round = millis();
        //yn = w × xn + (1 – w) × yn – 1
    }
  }
  //time0 holds trips so avg time to get back, prevTime holds loop times so print statement prints avg time to get back to the loop, avg time spent in the loop, and average total time.
  printf("avg delta: %f , avg loop: %f, avg total: %f\n", time0/(float)numberOfIterations, ((float)prevTime)/(float)numberOfIterations, (time0/(float)numberOfIterations)+(((float)prevTime)/(float)numberOfIterations));
  printf("max loop: %f, min loop: %f, max delt: %f, min delt: %f\n", max, min, maxdelt, mindelt);
}
