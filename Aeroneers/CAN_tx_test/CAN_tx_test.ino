#include <FlexCAN.h>

#define led 13
FlexCAN CANbus(500000);

static CAN_message_t msg, rxmsg;

void setup() {
  // put your setup code here, to run once:
  CANbus.begin();
  digitalWrite(led,1);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:

    digitalWrite(led,1);
    delay(1000);
    msg.len = 8;
    msg.id = 0x222;
    for( int idx=0; idx<8; ++idx ) {
      msg.buf[idx] = '0'+idx;
    }
    CANbus.write(msg);
    digitalWrite(led, 0);

}
