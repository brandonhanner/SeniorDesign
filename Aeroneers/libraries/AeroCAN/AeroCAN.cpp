#include "AeroCAN.h"

AeroCAN::AeroCAN(uint8_t id)
{
  my_id = (uint8_t) id;
}
void AeroCAN::setID(uint8_t id)
{
  my_id = id;
}
int AeroCAN::read(AeroCAN_message_t &msg)            //deconstruct the CAN message into a AeroCAN message
{
  if (Can0.read(inmsg))                              //check and see if we have a CAN frame available, if so proceed, otherwise abort.
  {
    uint32_t cmd_id = inmsg.id;

    cmd_id &= 0x7C0;                                //apply a mask with bits 10-6 (starting at 0) to filter out anything but the message type portion
    cmd_id >>= 6;                                    //shift the cmd_id right 6 times to align it with the msg_type enums

    msg.message_type = (msg_type) cmd_id;

    uint8_t target_address = inmsg.id;
    target_address &= 0x3F;                         //apply a mask with bits 5-0 (starting at 0) to filter out anything but the target address portion
    msg.target_address = target_address;
    int i = 0;
    for (i=0;i<inmsg.len;i++)
    {
       msg.data[i] = inmsg.buf[i];
    }
    msg.len = inmsg.len;
    return 1;
  }
  return 0;
}

int AeroCAN::send(const AeroCAN_message_t &msg)
{
  uint32_t id = (uint32_t) msg.message_type;
  id <<= 6;

  id |= msg.target_address;
  #ifdef DEBUG
  Serial.println(id);
  #endif
  //create the CAN message
  outmsg.id = id;
  int i = 0;
  for (i=0;i<msg.len;i++)
  {
    outmsg.buf[i] = msg.data[i];
  }
  outmsg.len = msg.len;

  if (Can0.write(outmsg))
  {
   return 1;
  }
  else return 0;
}

AeroCANcontroller::AeroCANcontroller(void) : AeroCAN((uint8_t)0)
{

}

uint8_t AeroCANcontroller::setTargetNode(uint8_t node_id, state_e target_state)
{
  tempmsg.target_address = node_id;
  tempmsg.message_type = SET_TARGET_NODE;
  tempmsg.data[0] = target_state;
  tempmsg.len = 1;

  return (uint8_t) this->send(tempmsg);
}
uint8_t AeroCANcontroller::setTargetGroup(uint8_t group_id, state_e target_state)
{
  tempmsg.target_address = group_id;
  tempmsg.message_type = SET_TARGET_GROUP;
  tempmsg.data[0] = target_state;
  tempmsg.len = 1;

  return (uint8_t) this->send(tempmsg);
}
uint8_t AeroCANnode::sendCurrentState(void)
{
  tempmsg.target_address = my_id;
  tempmsg.message_type = CURRENT_STATE;
  tempmsg.data[0] = (uint8_t) current_state;
  tempmsg.len = 1;

  return (uint8_t) this->send(tempmsg);
}
uint8_t AeroCANcontroller::setOpenTime(uint8_t node_id, uint16_t open_millis)
{
  tempmsg.target_address = node_id;
  tempmsg.message_type = SET_OPEN_TIME;
  tempmsg.data[0] = open_millis & 0xFF;
  tempmsg.data[1] = open_millis >> 8;
  tempmsg.len = 2;

  return (uint8_t) this->send(tempmsg);
}
uint8_t AeroCANcontroller::setCloseTime(uint8_t node_id, uint16_t close_millis)
{
  tempmsg.target_address = node_id;
  tempmsg.message_type = SET_CLOSE_TIME;
  tempmsg.data[0] = close_millis & 0xFF;
  tempmsg.data[1] = close_millis >> 8;
  tempmsg.len = 2;

  return (uint8_t) this->send(tempmsg);
}
uint8_t AeroCANcontroller::setOpenAngle(uint8_t node_id, uint16_t open_angle)
{
  tempmsg.target_address = node_id;
  tempmsg.message_type = SET_OPEN_ANGLE;
  tempmsg.data[0] = open_angle & 0xFF;
  tempmsg.data[1] = open_angle >> 8;
  tempmsg.len = 2;

  return (uint8_t) this->send(tempmsg);
}
uint8_t AeroCANcontroller::setCloseAngle(uint8_t node_id, uint16_t closed_angle)
{
  tempmsg.target_address = node_id;
  tempmsg.message_type = SET_CLOSE_ANGLE;
  tempmsg.data[0] = closed_angle & 0xFF;
  tempmsg.data[1] = closed_angle >> 8;
  tempmsg.len = 2;

  return (uint8_t) this->send(tempmsg);
}
uint8_t AeroCANcontroller::setRetries(uint8_t node_id, uint8_t retries)
{
  tempmsg.target_address = node_id;
  tempmsg.message_type = SET_RETRIES;
  tempmsg.data[0] = retries;
  tempmsg.len = 1;

  return (uint8_t) this->send(tempmsg);
}
uint8_t AeroCANcontroller::setTimeout(uint8_t node_id, uint16_t timeout_millis)
{
  tempmsg.target_address = node_id;
  tempmsg.message_type = SET_TIMEOUT;
  tempmsg.data[0] = timeout_millis & 0xFF;
  tempmsg.data[1] = timeout_millis >> 8;
  tempmsg.len = 2;

  return (uint8_t) this->send(tempmsg);
}
uint8_t AeroCANcontroller::setMinPulse(uint8_t node_id, uint16_t min_micros)
{
  tempmsg.target_address = node_id;
  tempmsg.message_type = SET_MIN_PULSE;
  tempmsg.data[0] = min_micros & 0xFF;
  tempmsg.data[1] = min_micros >> 8;
  tempmsg.len = 2;

  return (uint8_t) this->send(tempmsg);
}
uint8_t AeroCANcontroller::setMaxPulse(uint8_t node_id, uint16_t max_micros)
{
  tempmsg.target_address = node_id;
  tempmsg.message_type = SET_MAX_PULSE;
  tempmsg.data[0] = max_micros & 0xFF;
  tempmsg.data[1] = max_micros >> 8;
  tempmsg.len = 2;

  return (uint8_t) this->send(tempmsg);
}
uint8_t AeroCANcontroller::setGroupID(uint8_t node_id, uint8_t slot, uint8_t group_id)
{
  tempmsg.target_address = node_id;
  tempmsg.message_type = SET_GROUP_ID;
  tempmsg.data[0] = slot;
  tempmsg.data[1] = group_id;
  tempmsg.len = 2;

  return (uint8_t) this->send(tempmsg);
}
uint8_t AeroCANcontroller::clearGroupID(uint8_t node_id, uint8_t slot)
{
  tempmsg.target_address = node_id;
  tempmsg.message_type = CLEAR_GROUP_ID;
  tempmsg.data[0] = slot;
  tempmsg.len = 1;

  return (uint8_t) this->send(tempmsg);
}
uint8_t AeroCANcontroller::requestConfig(uint8_t node_id, uint8_t parameter_id)
{
return 1;
}
///////////////////////////////////////// A e r o C A N n o d e //////////////////////////////////////////
AeroCANnode::AeroCANnode(uint8_t id) : AeroCAN(id)
{
/*
1->14
2->15
3->16
4->17
5->18
*/
pinMode(14, INPUT);
pinMode(15, INPUT);
pinMode(16, INPUT);
pinMode(17, INPUT);
pinMode(18, INPUT);

uint8_t temp_id;

temp_id = 0;
temp_id |= (uint8_t) digitalRead(14);
temp_id |= ((uint8_t) digitalRead(15)) << 1;
temp_id |= ((uint8_t) digitalRead(16)) << 2;
temp_id |= ((uint8_t) digitalRead(17)) << 3;
temp_id |= ((uint8_t) digitalRead(18)) << 4;

this->setID(temp_id);

}

uint8_t AeroCANnode::respondConfig(uint8_t parameter_id)
{
return 1;
}

void AeroCANnode::open_surface(void)
{
  if (!servo.attached())                                              //if the servo is disabled
  {
    servo_open_start_time = millis();                                 //start the open timer
    servo.attach(SERVO_PIN,minPulse,maxPulse);                        //enable the servo
    servo.write(OPEN);                                                //and tell it to open
  }
}

void AeroCANnode::close_surface(void)
{
  if (!servo.attached())                                             //if the servo is disabled
  {
    servo_close_start_time = millis();                               //start the close timer
    servo.attach(SERVO_PIN,minPulse,maxPulse);                       //enable the servo
    servo.write(CLOSE);                                              //and tell it to close
  }
}

void AeroCANnode::openDebounceInterrupt()                                          //this is the function that is called upon the rising edge of the "opened" input
{
  if((long)(micros() - last_open_micros) >= debouncing_time * 1000)   //if the current time - last time we came to the routine is greater than the threshold proceed, otherwise skip
  {
    OPEN_ISR();                                                       //call our actual interrupt routine
    last_open_micros = micros();                                      //and set the last time we were here to now
  }
}
void AeroCANnode::closeDebounceInterrupt()                                         //this is the function that is called upon the rising edge of the "closed" input
{
  if((long)(micros() - last_closed_micros) >= debouncing_time * 1000) //if the current time - last time we came to the routine is greater than the threshold proceed, otherwise skip
  {
    CLOSE_ISR();                                                      //call our actual interrup routine
    last_closed_micros = micros();                                    //and set the last time we were here to now
  }
}

void AeroCANnode::CLOSE_ISR()                                                      //this is the actual ISR (Interrupt Service Routine) where we alter the state based on the inputs
{
  current_state = CLOSED_STATE;                                       //change the current state to CLOSED_STATE, it's important to do as little as possible during an interrupt.
                                                                      //if you need to do more processing, simply set a flag in the interrupt and let the main loop address it.
  current_attempts = 0;                                               //reset the current_attempts to zero

}

void AeroCANnode::OPEN_ISR()                                                       //this is the actual ISR (Interrupt Service Routine) where we alter the state based on the inputs
{
  current_state = OPEN_STATE;                                         //change the current state to OPEN_STATE, it's important to do as little as possible during an interrupt.
                                                                      //if you need to do more processing, simply set a flag in the interrupt and let the main loop address it.
  current_attempts = 0;                                               //reset the current_attempts to zero

}

//This is the function that reads the CANbus for commands. It parses the incoming message and decides whether to change the target state to open or closed..
//This will undergo extensive modification once the CAN application layer is fleshed out.
void AeroCANnode::checkCAN()
{
  if (AeroCAN::read(tempmsg))
  {
    switch (tempmsg.message_type)
    {

     case SET_TARGET_NODE:
     {
       #ifdef DEBUG
       Serial.println("SET_TARGET_NODE");
       #endif
      if (tempmsg.target_address == my_id)
     {
       target_state = (state_e) tempmsg.data[0];
       digitalWrite(LED_BUILTIN,tempmsg.data[0]);
     }
     break;
     }


     case SET_TARGET_GROUP:
     {
       #ifdef DEBUG
       Serial.println("SET_TARGET_GROUP");
       #endif
      int index = 0;
      for (index=0;index<8;index++)
      {
        if (tempmsg.target_address == group_ids[index])
        {
          target_state = (state_e) tempmsg.data[0];
          if ((state_e) tempmsg.data[0] == CLOSED_STATE)
          {
            digitalWrite(13,HIGH);
          }
          else digitalWrite(13,LOW);
          break;
        }
      }
     break;
     }


     case CURRENT_STATE:
     {
       #ifdef DEBUG
       Serial.println("CURRENT_STATE");
       #endif
      if (tempmsg.target_address == my_id)
     {
      //ignore
     }
     break;
     }


     case SET_OPEN_TIME:
     {
       #ifdef DEBUG
       Serial.println("SET_OPEN_TIME");
       #endif
      if (tempmsg.target_address == my_id)
     {
       uint16_t temp = tempmsg.data[1];//first put the high byte in
       temp <<=8; //shift it left
       temp |= tempmsg.data[0];  //or it with the low byte
       open_time = (uint32_t)temp;
     }

     break;
     }


     case SET_CLOSE_TIME:
     {
       #ifdef DEBUG
       Serial.println("SET_CLOSE_TIME");
       #endif
      if (tempmsg.target_address == my_id)
     {
       uint16_t temp = tempmsg.data[1];//first put the high byte in
       temp <<=8; //shift it left
       temp |= tempmsg.data[0];  //or it with the low byte
       close_time = (uint32_t)temp;
     }
     break;
     }


     case SET_OPEN_ANGLE:
     {
       #ifdef DEBUG
       Serial.println("SET_OPEN_ANGLE");
       #endif
      if (tempmsg.target_address == my_id)
     {
       uint16_t temp = tempmsg.data[1];//first put the high byte in
       temp <<=8; //shift it left
       temp |= tempmsg.data[0];  //or it with the low byte
       OPEN = (int)temp;
     }
     break;
     }


     case SET_CLOSE_ANGLE:
     {
       #ifdef DEBUG
       Serial.println("SET_CLOSE_ANGLE");
       Serial.println(tempmsg.target_address);
       Serial.println(my_id);

       #endif
      if (tempmsg.target_address == my_id)
     {
      uint16_t close_angle = tempmsg.data[1];//first put the high byte in
      close_angle <<=8; //shift it left
      close_angle |= tempmsg.data[0];  //or it with the low byte
      CLOSE = (int)close_angle;
      #ifdef DEBUG
      Serial.println(CLOSE);
      #endif
     }
     break;
     }


     case SET_RETRIES:
     {
       #ifdef DEBUG
       Serial.println("SET_RETRIES");
       #endif
      if (tempmsg.target_address == my_id)
     {
      attempts = tempmsg.data[0];
     }

     break;
     }


     case SET_TIMEOUT:
     {
       #ifdef DEBUG
       Serial.println("SET_TIMEOUT");
       #endif
      if (tempmsg.target_address == my_id)
     {
       uint16_t temp = tempmsg.data[1];//first put the high byte in
       temp <<=8; //shift it left
       temp |= tempmsg.data[0];  //or it with the low byte
       timeout = (uint16_t)temp;
     }
     break;
     }


     case SET_MIN_PULSE:
     {
       #ifdef DEBUG
       Serial.println("SET_MIN_PULSE");
       #endif
      if (tempmsg.target_address == my_id)
     {
       uint16_t temp = tempmsg.data[1];//first put the high byte in
       temp <<=8; //shift it left
       temp |= tempmsg.data[0];  //or it with the low byte
       minPulse = (int)temp;
     }
     break;
     }


     case SET_MAX_PULSE:
     {
       #ifdef DEBUG
       Serial.println("SET_MAX_PULSE");
       #endif
       if (tempmsg.target_address == my_id)
     {
       uint16_t temp = tempmsg.data[1];//first put the high byte in
       temp <<=8; //shift it left
       temp |= tempmsg.data[0];  //or it with the low byte
       maxPulse = (int)temp;
     }
     break;
     }


     case SET_GROUP_ID:
     {
       #ifdef DEBUG
       Serial.println("SET_GROUP_ID");
       #endif
      if (tempmsg.target_address == my_id)
     {
       group_ids[tempmsg.data[0]] = tempmsg.data[1];
     }
     break;
     }


     case CLEAR_GROUP_ID:
     {
       #ifdef DEBUG
       Serial.println("CLEAR_GROUP_ID");
       #endif
      if (tempmsg.target_address == my_id)
     {
       //group_ids[tempmsg.data[0]] = 9;
     }
     break;
     }


     case CONFIG_REQUEST:
     {
       #ifdef DEBUG
       Serial.println("CONFIG_REQUEST");
       #endif
      if (tempmsg.target_address == my_id)
     {
       //send config
     }
     break;
     }


     case CONFIG_RESPONSE:

     //ignore
     break;
    }

  }
}

void AeroCANnode::run(void)
{
  checkCAN();//check for CAN messages, set target_state and config parameters accordingly

  ////////////////////////////////////////////////////////// T A R G E T  I S  O P E N /////////////////////////////////////////////////////////////////////////

  if (target_state == OPEN_STATE)                                  //if we are targeting an open control surface..
  {
    switch (current_state)                                         //decide what were going to do based on the current state
    {
      case OPEN_STATE:                                             //we are already in the OPEN_STATE
        //digitalWrite(LED_BUILTIN,HIGH);                            //change the state of the onboard LED, just to debug the system
        servo.detach();
        break;

      case CLOSED_STATE:                                           //we are currently in the CLOSED_STATE
        //digitalWrite(LED_BUILTIN,LOW);                             //change the state of the onboard LED, just to debug the system
        open_surface();                                            //send the message to open the control surface
        start_time = millis();                                     //record the start time of the opening process, in ms since the system booted
        current_state = TRANSITIONING_OPEN_STATE;                  //change the state to TRANSITIONING_OPEN_STATE
        break;

      case TRANSITIONING_OPEN_STATE:                               //we are already in the TRANSITIONING_OPEN_STATE
        //digitalWrite(LED_BUILTIN,LOW);                             //change the state of the onboard LED, just to debug the system

        if((millis() - start_time) > timeout)                      //if we have exceeded the timeout threshold, and still haven't reached the target state
        {
          //current_state = UNKNOWN_STATE;                           //set the state to UNKNOWN_STATE, indicating an error has occurred
          #ifdef NO_LIMIT
          OPEN_ISR();
          #endif
        }
        break;

      case TRANSITIONING_CLOSED_STATE:                             //we are already in the TRANSITIONING_CLOSED_STATE
        //digitalWrite(LED_BUILTIN,LOW);                             //change the state of the onboard LED, just to debug the system
        open_surface();                                            //send the message to open the control surface
        start_time = millis();                                     //record the start time of the opening process, in ms since the system booted
        current_state = TRANSITIONING_OPEN_STATE;                  //change the state to TRANSITIONING_OPEN_STATE
        break;

      case UNKNOWN_STATE:
        digitalWrite(LED_BUILTIN,HIGH);                            //change the state of the onboard LED, just to debug the system
        if (current_attempts < attempts)                              //if we haven't tried to open the control surface more than "attempts"
        {
          open_surface();                                          //send the message to open the control surface
          current_attempts++;                                      //increment the "current_attempts" to keep track of how many times we tried to open it
          start_time = millis();                                   //record the start time of the process to open
          current_state = TRANSITIONING_OPEN_STATE;                //set the current_state to TRANSITIONING_OPEN_STATE
        }
        break;

      default:
        break;
    }
    //if (millis() - servo_open_start_time > open_time)             //if the servo open_time has been exceeded
    //{
    //  servo.detach();                                             //disable the servo, this conserves wear and tear on the servo
    //}
  }
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////// T A R G E T  I S  C L O S E D ///////////////////////////////////////////////////////////////////////

  else if (target_state == CLOSED_STATE)                           //if we are targeting a closed control surface..
  {
    switch (current_state)                                         //decide what were going to do based on the current state
    {
      case OPEN_STATE:                                             //we are already in the OPEN_STATE
        //digitalWrite(LED_BUILTIN,LOW);                             //change the state of the onboard LED, just to debug the system
        close_surface();                                           //send the message to close the control surface
        start_time = millis();                                     //record the start time of the opening process, in ms since the system booted
        current_state = TRANSITIONING_CLOSED_STATE;                //set the current_state to TRANSITIONING_CLOSED_STATE
        break;

      case CLOSED_STATE:                                           //we are currently in the CLOSED_STATE
        //digitalWrite(LED_BUILTIN,HIGH);                            //change the state of the onboard LED, just to debug the system
        servo.detach();
        break;

      case TRANSITIONING_OPEN_STATE:                               //we are already in the TRANSITIONING_OPEN_STATE
        //digitalWrite(LED_BUILTIN,LOW);                             //change the state of the onboard LED, just to debug the system
        close_surface();                                           //send the message to close the control surface
        start_time = millis();                                     //record the start time of the opening process, in ms since the system booted
        current_state = TRANSITIONING_CLOSED_STATE;                //set the current_state to TRANSITIONING_CLOSED_STATE
        break;

      case TRANSITIONING_CLOSED_STATE:                             //we are already in the TRANSITIONING_CLOSED_STATE
        //digitalWrite(LED_BUILTIN,LOW);                             //change the state of the onboard LED, just to debug the system

        if((millis() - start_time) > timeout)                      //if we have exceeded the timeout threshold, and still haven't reached the target state
        {
          //current_state = UNKNOWN_STATE;                           //set the current_state to UNKNOWN_STATE
          #ifdef NO_LIMIT
          CLOSE_ISR();
          #endif
        }
        break;

      case UNKNOWN_STATE:                                          //we are already in the UNKNOWN_STATE
        //digitalWrite(LED_BUILTIN,HIGH);
                                  //change the state of the onboard LED, just to debug the system
        if (current_attempts < attempts)                           //if we haven't tried to close the control surface more than "attempts"
        {
          close_surface();                                         //send the message to close the control surface
          start_time = millis();                                   //record the start time of the opening process, in ms since the system booted
          current_attempts++;                                              //increment the "current_attempts" to keep track of how many times we tried to open it
          current_state = TRANSITIONING_CLOSED_STATE;              //set the current_state to TRANSITIONING_CLOSED_STATE
        }
        break;

      default:
        break;
    }
    //if (millis() - servo_close_start_time > close_time)            //if the servo close_time has been exceeded
    //{
    //  servo.detach();                                              //disable the servo, this conserves wear and tear on the servo
    //}
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
