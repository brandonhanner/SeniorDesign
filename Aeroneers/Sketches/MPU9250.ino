/**
 * Sample program for the MPU9250 using SPI
 *
 * Sample rate of the AK8963 magnetometer is set at 100Hz. 
 * There are only two options: 8Hz or 100Hz so I've set it at 100Hz
 * in the library. This is set by writing to the CNTL1 register
 * during initialisation.
 *
 * Copyright (C) 2015 Brian Chen
 *
 * Open source under the MIT license. See LICENSE.txt.
 */

#include <SPI.h>
#include "IntervalTimer.h"
#include <Filters.h>
#include <MPU9250.h>
#include <stdio.h>

#define SPI_CLOCK 8000000  // 8MHz clock works.

#define SS_PIN   10
#define INT_PIN  25
#define LED      13

long lastMillis;
long newMillis;
long startMillis;
long endMillis;
float filterFrequency = 40.0;
long Resultant;
float angle;
FilterOnePole lowpassFilterX( LOWPASS, filterFrequency );
FilterOnePole lowpassFilterY( LOWPASS, filterFrequency );
#define WAITFORINPUT(){            \
  while(!Serial.available()){};  \
  while(Serial.available()){     \
    Serial.read();             \
  };                             \
}                                  \

MPU9250 mpu(SPI_CLOCK, SS_PIN);
IntervalTimer timer0;

void timerCallback0()
{
  float ax, ay, az;
   char buffer[100];
  // mpu.read_gyro();
  ax = mpu.accel_data[0];
  ay = mpu.accel_data[1];
  az = mpu.accel_data[2];
  Serial.print(ax);    Serial.print("  ");
  Serial.print(ay);    Serial.print("  ");
  Serial.print(az);    Serial.print("  ");

  Resultant = sqrt(pow(ax, 2) + pow(ay, 2));
  angle = round( atan2 (ay, ax) * 180/3.14159265 );
  sprintf(buffer, "%.4ld", Resultant);
  Serial.print(buffer);
    Serial.print("  ");
  Serial.print(angle);
     Serial.print("  ");
   
  startMillis = micros();
  mpu.read_all();
  lowpassFilterX.input( Resultant );
  //lowpassFilterY.input( mpu.accel_data[1] );
  endMillis = micros();
  newMillis =  micros();
  /*
  Serial.print(mpu.gyro_data[0]);   Serial.print('\t');
  Serial.print(mpu.gyro_data[1]);   Serial.print('\t');
  Serial.print(mpu.gyro_data[2]);   Serial.print('\t');
  Serial.print(mpu.accel_data[0]);  Serial.print('\t');
  Serial.print(mpu.accel_data[1]);  Serial.print('\t');
  Serial.print(mpu.accel_data[2]);  Serial.print('\t');
  Serial.print(mpu.mag_data[0]);    Serial.print('\t');
  Serial.print(mpu.mag_data[1]);    Serial.print('\t');
  Serial.print(mpu.mag_data[2]);    Serial.print('\t');
  Serial.println(mpu.temperature);
  */
  Serial.print(newMillis - lastMillis);
  Serial.print("  ");
  Serial.print(endMillis - startMillis);
  Serial.print("  ");
  Serial.print(lowpassFilterX.output());
  Serial.println();
  lastMillis =  micros();
}
void setup() {
  Serial.begin(115200);

  pinMode(INT_PIN, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  SPI.begin();


  mpu.init(true);

  mpu.calib_acc();
  mpu.calib_mag();
  timer0.begin(timerCallback0, 10000); // 5 seconds

}

void loop() {
  
}
