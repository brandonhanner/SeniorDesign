#ifndef __DATAACQ_H__
#define __DATAACQ_H__

#include <SPI.h>
#include "IntervalTimer.h"
#include <Filters.h>
#include <MPU9250.h>
#include <stdio.h>

#define SPI_CLOCK 8000000  // 8MHz clock works.

#define SS_PIN   10
#define INT_PIN  25
#define LED      13


float filterFrequency = 40.0;
FilterOnePole lowpassFilterX( LOWPASS, filterFrequency );
IntervalTimer timer0;
MPU9250 mpu(SPI_CLOCK, SS_PIN);

class DAQ {
  float ax, ay, az,angle;
  double Resultant;
  char buffer[100];
  long lastMillis;
  long newMillis;
  long startMillis;
  long endMillis;

public:
  void timerCallback0();
  void setup();
  void loop();
};

#endif
