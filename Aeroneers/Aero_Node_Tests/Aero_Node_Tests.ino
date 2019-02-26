#include<stdint.h>
#include<FlexCAN.h>

CAN_message_t msg;
int x;
void setup() {
  // put your setup code here, to run once:
  Can0.begin(1000000);
  Serial.begin(9600);
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  msg.buf[0] ^= 1;
  msg.len = 1;
  msg.timeout = 100;
  msg.id = 1;
  x = Can0.write(msg);
  if (msg.buf[0]==1)
  {
    Serial.println("OPEN");
    digitalWrite(LED_BUILTIN,HIGH);
  }
  else 
  {
    Serial.println("CLOSE");
    digitalWrite(LED_BUILTIN,LOW);
  }
  
  delay(1000);
}
