#include<stdint.h>
#include<FlexCAN.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//////////////////////////////////////////////////////////////// V A R I A B L E S /////////////////////////////////////////////////////////////////////////////
typedef enum
{
  OPEN_STATE,                 //we assume the wing is in the open state
  CLOSED_STATE,               //we assume the wing is in the closed state
  TRANSITIONING_OPEN_STATE,   //we are in the process of waiting for the wing to move
  TRANSITIONING_CLOSED_STATE, //we are in the process of waiting for the wing to move
  UNKNOWN_STATE               //we dont know the state of the wing
} state_e;

//these are the state variables that the node uses to keep track of whats going within its scope of concern.
//the current state is the state that the board is currently in, it is determined by the inputs from the control surfaces. 
//the target state is what the board is attempting to achieve, it is determined by the messages coming in from the CAN interface. 

volatile state_e current_state; //state variable for the current state of the system
volatile state_e target_state; //state variable for the target state of the system

//This block of variables is used within the ISR for software debouncing of the inputs. debouncing time is the threshold that you want to set for your system.
long debouncing_time = 15;     //microseconds we want to ignore the state of the button until we accept a state.
volatile uint32_t last_closed_micros=0;
volatile uint32_t last_open_micros=0;

//This block of variables is for keeping track of timing when transitioning between states. 
//We do this so that, if for example we want the control surface to open, and the control
//surface takes longer than "timeout" ms to open we try a certain number of times (which we keep in "attempts"),
//and then we can go into an unknown state because we know something is wrong. 

uint32_t timeout = 100;         // time in ms that we want to call an operation timed out
volatile uint32_t start_time;   //time that the transition was started
volatile uint32_t current_time; //current time
volatile int attempts = 0;      // retry counter
int tries = 5;                  //number of attempts that we want the node to make to meet the target state
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////// R E A D  C A N  F U N C T I O N //////////////////////////////////////////////////////////////////////

CAN_message_t recmsg;  //this is the structure we receive CAN messages into, 
CAN_message_t sendmsg; //this is the structure we form CAN messages with, and send with Can0

//this is the function that reads the CANbus for commands. It parses the incoming message and decides whether to change the target state to open or closed..
state_e readCAN()
{
  if (Can0.read(recmsg))
  {
    if (recmsg.buf[0] == 1) return OPEN_STATE;
    else if (recmsg.buf[0] == 0) return CLOSED_STATE;
  }
  return target_state;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////  I N T E R R U P T  S E R V I C E  R O U T I N E S  //////////////////////////////////////////////////////////////
void openDebounceInterrupt()
{
  if((long)(micros() - last_open_micros) >= debouncing_time * 1000) {
    OPEN_ISR();
    last_open_micros = micros();
  }
}
void closeDebounceInterrupt()
{
  if((long)(micros() - last_closed_micros) >= debouncing_time * 1000) {
    CLOSE_ISR();
    last_closed_micros = micros();
  }
}
void CLOSE_ISR()
{
  current_state = CLOSED_STATE;
  attempts = 0;
}

void OPEN_ISR()
{
  current_state = OPEN_STATE;
  attempts = 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////// A R D U I N O  S E T U P  F U N C T I O N ////////////////////////////////////////////////////////////////
void setup() 
{
  // put your setup code here, to run once:
  Can0.begin(1000000);                              //start the CAN interface at 1Mbps baud
  pinMode(LED_BUILTIN,OUTPUT);                      //declare the LED as an output
  current_state = UNKNOWN_STATE;                    //start the node with a current state of unknown
  target_state = CLOSED_STATE;                      //start the node with a target state of closed 
  attachInterrupt(7,openDebounceInterrupt,RISING);  //attach the openDebounceInterrupt function to the rising edge of input on pin 7
  attachInterrupt(8,closeDebounceInterrupt,RISING); //attach the closeDebounceInterrupt function to the rising edge of input on pin 8
  Serial.begin(115200);                             //Start the Serial terminal, this is for debugging only.
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////// S E R V O  C O N T R O L   F U N C T I O N S //////////////////////////////////////////////////////////////
void open_surface()
{
  //send_pwm_open();
}

void close_surface()
{
  //send_pwm_close();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////// M A I N  L O O P ///////////////////////////////////////////////////////////////////////////
void loop() 
{
  // put your main code here, to run repeatedly:
  target_state = readCAN();//check for CAN messages, set target_state accordingly
  
  ////////////////////////////////////////////////////////// T A R G E T  I S  O P E N /////////////////////////////////////////////////////////////////////////
  if (target_state == OPEN_STATE)//if we want wing open
  {
    switch (current_state)
    {
      case OPEN_STATE:
        digitalWrite(LED_BUILTIN,HIGH);
        break;

      case CLOSED_STATE:
        digitalWrite(LED_BUILTIN,LOW);
        //open_surface();
        start_time = current_time;
        current_state = TRANSITIONING_OPEN_STATE;
        break;

      case TRANSITIONING_OPEN_STATE:
        digitalWrite(LED_BUILTIN,LOW);
        if((current_time - start_time) > timeout)//if we timed out
        {
          current_state = UNKNOWN_STATE;
        }
        break;

      case TRANSITIONING_CLOSED_STATE:
        digitalWrite(LED_BUILTIN,LOW);
        //open_surface();
        start_time = current_time;
        current_state = TRANSITIONING_OPEN_STATE;
        break;

      case UNKNOWN_STATE:
        digitalWrite(LED_BUILTIN,LOW);
        if (attempts < tries)
        {
          //open_surface();
          attempts++;
          start_time = current_time;
          current_state = TRANSITIONING_OPEN_STATE;
        }
        break;

      default:
        break;
    }

  }
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////// T A R G E T  I S  C L O S E D ///////////////////////////////////////////////////////////////////////
  else if (target_state == CLOSED_STATE)//if we want wing closed
  {
    switch (current_state)
    {
      case OPEN_STATE:
        digitalWrite(LED_BUILTIN,LOW);
        //close_surface();
        start_time = current_time;
        current_state = TRANSITIONING_CLOSED_STATE;
        break;

      case CLOSED_STATE:
        digitalWrite(LED_BUILTIN,HIGH);
        break;

      case TRANSITIONING_OPEN_STATE:
        digitalWrite(LED_BUILTIN,LOW);
        //close_surface();
        start_time = current_time;
        current_state = TRANSITIONING_CLOSED_STATE;
        break;

      case TRANSITIONING_CLOSED_STATE:
        digitalWrite(LED_BUILTIN,LOW);
        if((current_time - start_time) > timeout)//if we timed out
        {
          current_state = UNKNOWN_STATE;
        }
        break;

      case UNKNOWN_STATE:
        digitalWrite(LED_BUILTIN,LOW);
        if (attempts < tries)
        {
          //close_surface();
          start_time = current_time;
          attempts++;
          current_state = TRANSITIONING_CLOSED_STATE;
        }
        break;

      default:
        break;
    }
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
