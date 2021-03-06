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
#include <MPU9250.h>

#define SPI_CLOCK 8000000  // 8MHz clock works.

#define SS_PIN   15
#define INT_PIN  25
#define LED      13

#define WAITFORINPUT(){            \
  while(!Serial.available()){};  \
  while(Serial.available()){     \
    Serial.read();             \
  };                             \
}                                  \

MPU9250 mpu(SPI_CLOCK, SS_PIN);

void setup() {
  Serial.begin(115200);

  pinMode(INT_PIN, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  SPI.begin();

  Serial.println("Press any key to continue");
  WAITFORINPUT();

  mpu.init(true);

  uint8_t wai = mpu.whoami();
  if (wai == 0x71){
    Serial.println("Successful connection");
  }
  else{
    Serial.print("Failed connection: ");
    Serial.println(wai, HEX);
  }

  uint8_t wai_AK8963 = mpu.AK8963_whoami();
  if (wai_AK8963 == 0x48){
    Serial.println("Successful connection to mag");
  }
  else{
    Serial.print("Failed connection to mag: ");
    Serial.println(wai_AK8963, HEX);
  }

  mpu.calib_acc();
  mpu.calib_mag();

  Serial.println("Send any char to begin main loop.");
  WAITFORINPUT();
}

void loop() {
  // various functions for reading
  // mpu.read_mag();
  // mpu.read_acc();
  // mpu.read_gyro();
int i=0;
int millis1 = millis();

for (i=0;i<1000;i++)
{
  mpu.read_all();
}
int millis2 = millis();

Serial.println(millis2-millis1);


  

  

}
