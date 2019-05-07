#include <PWMServo.h>
//#include "/Users/Casey/Documents/Arduino/libraries/PWMServo-master/PWMServo.h"

PWMServo myservo;  // create servo object to control a servo
#define SERVO_PIN 23
void setup() {
  // put your setup code here, to run once:
  myservo.attach(SERVO_PIN,900,2400);
}

void loop() {
  // put your main code here, to run repeatedly:
  //myservo.attach(SERVO_PIN,900,2400);
  myservo.write(0);
  delay(1000);
  //myservo.detach();
  //delay(3000);
  /*
  myservo.attach(SERVO_PIN,900,2400);
  myservo.write(180);
  delay(500);
  myservo.detach();
  delay(3000);
  */
}
