// -------------------------------------------------------------
// an application layer library for Active Aero based on Colins FlexCAN library
//
#ifndef __AEROCAN_H__
#define __AEROCAN_H__

#include<stdint.h>
#include "flexcan.h"
#include<PWMServo2.h>
#define DEBUG
typedef enum
{
  SET_TARGET_NODE,  //0
  SET_TARGET_GROUP, //1
  CURRENT_STATE,    //2
  SET_OPEN_TIME,    //3
  SET_CLOSE_TIME,   //4
  SET_OPEN_ANGLE,   //5
  SET_CLOSE_ANGLE,  //6
  SET_RETRIES,      //7
  SET_TIMEOUT,      //8
  SET_MIN_PULSE,    //9
  SET_MAX_PULSE,    //10
  SET_GROUP_ID,     //11
  CLEAR_GROUP_ID,   //12
  CONFIG_REQUEST,   //13
  CONFIG_RESPONSE,  //14
  UNIMPLEMENTED     //15
} msg_type;

typedef enum
{
  CLOSED_STATE,                           //we assume the wing is in the closed state
  OPEN_STATE,                             //we assume the wing is in the open state
  TRANSITIONING_CLOSED_STATE,             //we are in the process of waiting for the wing to move
  TRANSITIONING_OPEN_STATE,               //we are in the process of waiting for the wing to move
  UNKNOWN_STATE                           //we dont know the state of the wing
} state_e;


typedef struct
{
  msg_type message_type;
  uint8_t target_address;
  uint8_t data[8];
  uint8_t len;
}AeroCAN_message_t;

#define NODE 0
#define CONTROLLER 1
#define SERVO_PIN 23                       //pin on the teensy 3.2 that the signal for the servo is derived from


class AeroCAN
{
protected:
  //common variables
  uint8_t my_id;
  AeroCAN_message_t tempmsg;
  CAN_message_t inmsg;  //this is the structure we receive CAN messages into,
  CAN_message_t outmsg; //this is the structure we form CAN messages with, and send with Can0

public:
  AeroCAN(uint8_t id);
  int send(const AeroCAN_message_t &msg); //deconstruct the AeroCAN message into a CAN message
  int read(AeroCAN_message_t &msg); //deconstruct the CAN message into a AeroCAN message
};

class AeroCANcontroller : public AeroCAN
{
private:
  //mainboard variables


public:

  //mainboard functions
  AeroCANcontroller(void);
  void checkCAN(void);
  uint8_t setTargetNode(uint8_t node_id, state_e target_state);
  uint8_t setTargetGroup(uint8_t group_id, state_e target_state);
  uint8_t setOpenTime(uint8_t node_id, uint16_t open_millis);
  uint8_t setCloseTime(uint8_t node_id, uint16_t close_millis);
  uint8_t setOpenAngle(uint8_t node_id, uint16_t open_angle);
  uint8_t setCloseAngle(uint8_t node_id, uint16_t closed_angle);
  uint8_t setRetries(uint8_t node_id, uint8_t retries);
  uint8_t setTimeout(uint8_t node_id, uint16_t timeout_millis);
  uint8_t setMinPulse(uint8_t node_id, uint16_t min_micros);
  uint8_t setMaxPulse(uint8_t node_id, uint16_t max_micros);
  uint8_t setGroupID(uint8_t node_id, uint8_t slot, uint8_t group_id);
  uint8_t clearGroupID(uint8_t node_id, uint8_t slot);
  uint8_t requestConfig(uint8_t node_id, uint8_t parameter_id);
};


class AeroCANnode : public AeroCAN
{
private:

  //node variables

  uint8_t group_ids[8] = {255,255,255,255,255,255,255,255};

  volatile uint32_t start_time;                    //time that the transition was started
  volatile int current_attempts = 0;               //number of times we have attempted to meet the desired state
  uint16_t timeout=600;
  uint8_t attempts = 5;
  volatile int OPEN = 0;                           //define the target in degrees that the servo needs to move to for the open state
  volatile int CLOSE = 180;                        //define the target in degrees that the servo needs to move to for the closed state
  volatile int minPulse = 640;                     //minimum pulse width in microseconds that the servo can read
  volatile int maxPulse = 2250;                    //maximum pulse width in microseconds that the servo can read
  volatile uint32_t open_time = 500;               //time in milliseconds that it SHOULD take the wing to make this maneuver
  volatile uint32_t close_time = 500;              //time in milliseconds that it SHOULD take the wing to make this maneuver
  volatile uint32_t servo_open_start_time = 0;     //time in milliseconds in which the servo began to open
  volatile uint32_t servo_close_start_time = 0;    //time in milliseconds in which the servo began to close

  PWMServo servo;

  //This block of variables is used within the ISR(Interrupt Service Routine) for software debouncing of the inputs. Debouncing time is the threshold of time in milliseconds that you want to set for your system.
  long debouncing_time = 15;                                            //microseconds we want to ignore the state of the button until we accept a state.
  volatile uint32_t last_closed_micros=0;
  volatile uint32_t last_open_micros=0;



  volatile state_e current_state = UNKNOWN_STATE;                  //state variable for the current state of the system
  volatile state_e target_state = CLOSED_STATE;                   //state variable for the target state of the system



public:
  //node functions
  AeroCANnode(uint8_t id);
  uint8_t sendCurrentState(void);
  uint8_t respondConfig(uint8_t parameter_id);
  void open_surface(void);
  void close_surface(void);
  void checkCAN(void);
  void run(void);
  void openDebounceInterrupt(void);
  void closeDebounceInterrupt(void);
  void OPEN_ISR(void);
  void CLOSE_ISR(void);

};

#endif // __AEROCAN_H__
