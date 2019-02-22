//#define receive_CAN 0
#define transmit_CAN 0
#ifdef transmit_CAN//-------------TRANSMITTER -------------------

#include <FlexCAN.h>
//#include <kinetis_flexcan.h>
#define BUTTON 0
FlexCAN CANbus(500000);
CAN_message_t msg;
int x;

void setup() {
  CANbus.begin();
  pinMode(BUTTON,INPUT_PULLUP);   // Button to GND
  pinMode(LED_BUILTIN,OUTPUT);    // LED
  Serial.begin(9600);
}

void loop() {
  delay(10);
  digitalWrite(LED_BUILTIN,HIGH); //blink while transmitting
  msg.buf[0]= 1;
  msg.len = 1;
  msg.timeout=100;
  msg.id = 1;
  x=CANbus.write(msg);
  Serial.write(x+48); //1:success, 0:fail
  digitalWrite(LED_BUILTIN,LOW);
  delay(50);
}
#endif
#ifdef receive_CAN//-------------RECEIVER -------------------

#include <FlexCAN.h>
//#include <kinetis_flexcan.h>
FlexCAN CANbus(500000);
CAN_message_t rxmsg;

void setup() {
  CANbus.begin();
  pinMode(LED_BUILTIN,OUTPUT);    // LED
  digitalWrite(LED_BUILTIN,HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  if (CANbus.read(rxmsg)){
    digitalWrite(LED_BUILTIN,HIGH); //blink LED
    delay(100);
    digitalWrite(LED_BUILTIN,LOW);
    delay(10);
  }
}
#endif
