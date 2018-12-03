#include "IMU.h"
void myIMU::updateIMU()
{
  bool not_done = true;
  while(not_done)
  {
    if (imu->checkNewAccelGyroData())  // data ready interrupt is detected
    {
        imu->readAccelerometer(xAccel, yAccel, zAccel);
        imu->readGyrometer(xRotation, yRotation, zRotation);
        if(imu->checkNewMagData()) {

            imu->readMagnetometer(xMag, yMag, zMag);
        }
        //imu->readMagnetometer(xMag, yMag, zMag);//this is for the master version
        temp = imu->readTemperature();
        not_done = false;
    }
  }


}
void myIMU::updateAccel()
{
  imu->readAccelerometer(xAccel, yAccel, zAccel);
}
void myIMU::calibrateAccelerometer()
{
  int count = 0;
  xAccelOffset = 0.0;
  yAccelOffset = 0.0;
  zAccelOffset = 0.0;
  while(count < 100)
  {
    if (imu->checkNewAccelGyroData())  // data ready interrupt is detected
    {
        imu->readAccelerometer(xAccel, yAccel, zAccel);
        imu->readGyrometer(xRotation, yRotation, zRotation);
        if(imu->checkNewMagData()) {

            imu->readMagnetometer(xMag, yMag, zMag);
        }
        //imu->readMagnetometer(xMag, yMag, zMag);//this is for the master version
        temp = imu->readTemperature();
        count++;
        xAccelOffset+=xAccel;
        yAccelOffset+=yAccel;
        zAccelOffset+=zAccel;
    }

  }
  xAccelOffset/=100.0;
  yAccelOffset/=100.0;
  zAccelOffset/=100.0;
}
float myIMU::getXaccel()
{
  return xAccel-xAccelOffset;
}
float myIMU::getYaccel()
{
  return yAccel-yAccelOffset;
}
float myIMU::getZaccel()
{
  return zAccel-zAccelOffset;
}

float myIMU::getXrotation()
{
  return xRotation;
}
float myIMU::getYrotation()
{
  return yRotation;
}
float myIMU::getZrotation()
{
  return zRotation;
}

float myIMU::getXmag()
{
  return xMag;
}
float myIMU::getYmag()
{
  return yMag;
}
float myIMU::getZmag()
{
  return zMag;
}
