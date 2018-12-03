#ifndef __IMU_H
#define __IMU_H 201609

//#include <MPU9250_Master_I2C.h>
#include <MPU9250_Passthru.h>
#include <stdio.h>
#include <wiringPi.h>
#include <errmsg.h>

using namespace std;


static const MPUIMU::Gscale_t GSCALE     = MPUIMU::GFS_250DPS;
static const MPUIMU::Ascale_t ASCALE     = MPUIMU::AFS_2G;
static const MPU9250::Mscale_t MSCALE    = MPU9250::MFS_16BITS;
static const MPU9250::Mmode_t MMODE      = MPU9250::M_100Hz;
static const uint8_t SAMPLE_RATE_DIVISOR = 0x00;

//static MPU9250_Master_I2C imu(ASCALE, GSCALE, MSCALE, MMODE, SAMPLE_RATE_DIVISOR);

class myIMU {
  public:
     myIMU()
     {
       // Setup WirinPi
       wiringPiSetup();
       delay(100);
       imu = new MPU9250_Passthru(ASCALE, GSCALE, MSCALE, MMODE, SAMPLE_RATE_DIVISOR);
       // Start the MPU9250
       switch (imu->begin())
       {

           case MPUIMU::ERROR_IMU_ID:
               errmsg("Bad IMU device ID");
               break;
           case MPUIMU::ERROR_MAG_ID:
               errmsg("Bad magnetometer device ID");
               break;
           case MPUIMU::ERROR_SELFTEST:
               errmsg("Failed self-test");
               break;
           default:
               printf("MPU6050 online!\n");
               break;
       }
       xAccel = 0.0;
       yAccel = 0.0;
       zAccel = 0.0;
       xAccelOffset = 0.0;
       yAccelOffset = 0.0;
       zAccelOffset = 0.0;

       xRotation = 0.0;
       yRotation = 0.0;
       zRotation = 0.0;

       xMag = 0.0;
       yMag = 0.0;
       zMag = 0.0;

       temp = 0.0;
     }

     void updateIMU(); //updates all IMU feeds
     void updateAccel(); //updates only the accelerometer portion
     void calibrateAccelerometer();

     float getXaccel();
     float getYaccel();
     float getZaccel();

     float getXrotation();
     float getYrotation();
     float getZrotation();

     float getXmag();
     float getYmag();
     float getZmag();

   private:
     MPU9250_Passthru* imu;

     float xAccel;
     float yAccel;
     float zAccel;
     float xAccelOffset;
     float yAccelOffset;
     float zAccelOffset;

     float xRotation;
     float yRotation;
     float zRotation;

     float xMag;
     float yMag;
     float zMag;

     float temp;

 };
 #endif
