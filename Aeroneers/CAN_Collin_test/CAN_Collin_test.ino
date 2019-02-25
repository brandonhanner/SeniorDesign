#define receive_CAN 0
//#define transmit_CAN 0
#ifdef transmit_CAN//-------------TRANSMITTER -------------------

#include <FlexCAN.h>
CAN_message_t msg;
int x;

void setup() {
  Can0.begin();
  pinMode(LED_BUILTIN,OUTPUT);    // LED
  Serial.begin(9600);
}

void loop() {
  delay(10);
  digitalWrite(LED_BUILTIN,HIGH); //blink while transmitting
  msg.buf[0]++;
  msg.len = 1;
  msg.timeout=100;
  msg.id = 34;
  x=Can0.write(msg);
  Serial.write(x+48); //1:success, 0:fail
  digitalWrite(LED_BUILTIN,LOW);
  delay(15);
}
#endif
#ifdef receive_CAN//-------------RECEIVER -------------------

#include <FlexCAN.h>
CAN_message_t rxmsg;

void setup() {
  Can0.begin();
  pinMode(LED_BUILTIN,OUTPUT);    // LED
  digitalWrite(LED_BUILTIN,HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  while (Can0.available()){
    Can0.read(rxmsg);
    digitalWrite(LED_BUILTIN,HIGH); //blink LED
    delay(100);
    digitalWrite(LED_BUILTIN,LOW);
    delay(100);
  }
}
#endif
