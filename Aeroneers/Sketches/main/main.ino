#include <SPI.h>
#include "IntervalTimer.h"
#include <Filters.h>
#include <MPU9250.h>
#include <stdio.h>
#include "AeroCAN.h"

AeroCANcontroller controller;

#define SPI_CLOCK 8000000  // 8MHz clock works.

#define SS_PIN   10
#define INT_PIN  25
#define LED      13
#define userButton 24

float filterFrequency = 40.0;

double Resultant;
float angle;

bool buttonPressed = false;

FilterOnePole lowpassFilterX( LOWPASS, filterFrequency );

MPU9250 mpu(SPI_CLOCK, SS_PIN);
IntervalTimer timer0, timer1;

void timerCallback0()
{
  float ax, ay, az;
  ax = mpu.accel_data[0];
  ay = mpu.accel_data[1];
  az = mpu.accel_data[2];

  Resultant = sqrt((pow(ax, 2)) + (pow(ay, 2)));
  angle = round( atan2 (ay, ax) * 180/3.14159265 );

  if( angle < 0){
    angle += 360;
  }
   
  mpu.read_all();
  lowpassFilterX.input( Resultant );


}

void timerCallback1()
{
  if(buttonPressed == true){
    Serial.println("button ON");
  }
  else{
   Serial.println("button OFF");
  }

  Serial.println(Resultant);
  Serial.println(angle);
  
  if((Resultant > 0.7 && (angle > 290 || angle < 250)) || ( buttonPressed ) ){
    controller.setTargetNode((uint8_t)1, CLOSED_STATE);
  }
  else{
    controller.setTargetNode((uint8_t)1, OPEN_STATE);
  }
  
}

void setup() {
  Serial.begin(115200);
  Can0.begin(1000000);
  pinMode(INT_PIN, INPUT);

  SPI.begin();


  mpu.init(true);

  mpu.calib_acc();
  mpu.calib_mag();
  timer0.begin(timerCallback0, 10000); // 5 seconds

  pinMode(userButton, INPUT);
  timer1.begin(timerCallback1, 10000); // 5 seconds

}

void loop() {
  if (digitalRead(userButton)){
    buttonPressed = true;
  }
  else{
    buttonPressed = false;
  }
}
