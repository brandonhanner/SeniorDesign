#include<AeroCAN.h>

AeroCANcontroller controller;

uint8_t node_id = 1;

bool isBusy,request;

long lasttime;

void testCommands()
{
  request = true;
  if (micros() - lasttime > 20)
  {
    isBusy = false;
    lasttime = micros();
  }
  else isBusy = true;
  
  
}

void setup() {
  // put your setup code here, to run once:
  Can0.begin(1000000);
  Serial.begin(115200);
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  attachInterrupt(24, testCommands,RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  controller.setTargetNode(node_id,OPEN_STATE);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  controller.setTargetNode(node_id, CLOSED_STATE);
  digitalWrite(LED_BUILTIN,LOW);
  delay(500);*/
  if(request && !isBusy)
  {
    controller.setTargetNode(node_id, CLOSED_STATE);
    delay(1); 
    controller.setTargetGroup((uint8_t) 0, OPEN_STATE);
    delay(1);
    controller.setOpenTime(node_id, (uint16_t) 100);
    delay(1);
    controller.setCloseTime(node_id, (uint16_t) 100);
    delay(1);
    controller.setOpenAngle(node_id, (uint16_t) 25);
    delay(1);
    controller.setCloseAngle(node_id, (uint16_t) 130);
    delay(1);
    controller.setRetries(node_id, (uint8_t) 4);
    delay(1);
    controller.setTimeout(node_id, (uint16_t) 800);
    delay(1);
    controller.setMinPulse(node_id, (uint16_t) 61);
    delay(1);
    controller.setMaxPulse(node_id, (uint16_t) 2200);
    delay(1);
    controller.setGroupID(node_id, (uint8_t) 0, (uint8_t) 2);
    delay(1);
    controller.clearGroupID(node_id, (uint8_t) 0);
    delay(1);
    controller.requestConfig(node_id, (uint8_t) 1);
    delay(1);
    request = false;
  }
  
}
