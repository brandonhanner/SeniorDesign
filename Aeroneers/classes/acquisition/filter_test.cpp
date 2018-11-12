#include "IMU.h"
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include "math.h"
#include <stdlib.h>

int main()
{
  myIMU imu;
  imu.updateIMU();
  printf("%f\n",imu.getXaccel());
  imu.calibrateAccelerometer();
  float xValue=0.0;
  float yValue=0.0;
  float vector = 0.0;
  float smoothing = 3.0;
  uint32_t timeSinceLastSample = 0;
  uint32_t lastTime = 0;
  float dot,det,angle;

  while (true)
  {
    timeSinceLastSample = millis() - lastTime;
    imu.updateIMU();
    xValue += (imu.getXaccel() - xValue) / (smoothing / timeSinceLastSample);
    yValue += (imu.getYaccel() - yValue) / (smoothing / timeSinceLastSample);

    vector = sqrt((xValue*xValue)+(yValue*yValue));
    dot = -1*xValue + 0*yValue;      //# dot product
    det = -1*yValue - 0*xValue;      //# determinant
    angle = atan2(det, dot);  //# atan2(y, x) or atan2(sin, cos)
    if (angle < 0) angle += 2*M_PI;
    lastTime = millis();
    printf("%f @ %fdeg\n", vector,angle*(180/M_PI));
  }

}
