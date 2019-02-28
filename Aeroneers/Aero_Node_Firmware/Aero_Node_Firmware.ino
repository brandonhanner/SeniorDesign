#include<stdint.h>
#include<FlexCAN.h>
#include<PWMServo2.h>

//////////////////////////////////////////////////////////////// S T A T E  V A R I A B L E S /////////////////////////////////////////////////////////////////////
typedef enum
{
  OPEN_STATE,                             //we assume the wing is in the open state
  CLOSED_STATE,                           //we assume the wing is in the closed state
  TRANSITIONING_OPEN_STATE,               //we are in the process of waiting for the wing to move
  TRANSITIONING_CLOSED_STATE,             //we are in the process of waiting for the wing to move
  UNKNOWN_STATE                           //we dont know the state of the wing
} state_e;

//These are the state variables that the node uses to keep track of whats going within its scope of concern.
//The current state is the state that the board is currently in, it is determined by the inputs from the control surfaces.
//The target state is what the board is attempting to achieve, it is determined by the messages coming in from the CAN interface.

volatile state_e current_state;           //state variable for the current state of the system
volatile state_e target_state;            //state variable for the target state of the system

//This block of variables is for keeping track of timing when transitioning between states.
//We do this so that, if for example we want the control surface to open, and the control
//surface takes longer than "timeout" ms to open we try a certain number of times (which we keep in "attempts"),
//and then we can go into an unknown state because we know something is wrong.

uint32_t timeout = 500;                   //time in ms that we want to call an operation timed out
volatile uint32_t start_time;             //time that the transition was started
volatile int current_attempts = 0;        //number of times we have attempted to meet the desired state
int attempts = 4;                         //number of attempts that we want the node to make to meet the target state
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////// R E A D  C A N  F U N C T I O N //////////////////////////////////////////////////////////////////////

CAN_message_t recmsg;  //this is the structure we receive CAN messages into,
CAN_message_t sendmsg; //this is the structure we form CAN messages with, and send with Can0

//This is the function that reads the CANbus for commands. It parses the incoming message and decides whether to change the target state to open or closed..
//This will undergo extensive modification once the CAN application layer is fleshed out.
state_e readCAN()
{
  if (Can0.read(recmsg))                              //check and see if we have a CAN frame available, if so proceed, otherwise abort.
  {
    if (recmsg.buf[0] == 1) return OPEN_STATE;        //if the first byte of the data section of the CAN frame is a 1, set the target_state to OPEN_STATE
    else if (recmsg.buf[0] == 0) return CLOSED_STATE; //if the first byte of the data section of the CAN frame is a 0, set teh target_state to CLOSED_STATE
  }
  return target_state;                                //if for some reason the byte is neither, dont modify the state
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////  I N T E R R U P T  S E R V I C E  R O U T I N E S  //////////////////////////////////////////////////////////////

//This block of variables is used within the ISR(Interrupt Service Routine) for software debouncing of the inputs. Debouncing time is the threshold of time in milliseconds that you want to set for your system.
long debouncing_time = 15;                                            //microseconds we want to ignore the state of the button until we accept a state.
volatile uint32_t last_closed_micros=0;
volatile uint32_t last_open_micros=0;

void openDebounceInterrupt()                                          //this is the function that is called upon the rising edge of the "opened" input
{
  if((long)(micros() - last_open_micros) >= debouncing_time * 1000)   //if the current time - last time we came to the routine is greater than the threshold proceed, otherwise skip
  {
    OPEN_ISR();                                                       //call our actual interrupt routine
    last_open_micros = micros();                                      //and set the last time we were here to now
  }
}
void closeDebounceInterrupt()                                         //this is the function that is called upon the rising edge of the "closed" input
{
  if((long)(micros() - last_closed_micros) >= debouncing_time * 1000) //if the current time - last time we came to the routine is greater than the threshold proceed, otherwise skip
  {
    CLOSE_ISR();                                                      //call our actual interrup routine
    last_closed_micros = micros();                                    //and set the last time we were here to now
  }
}
void CLOSE_ISR()                                                      //this is the actual ISR (Interrupt Service Routine) where we alter the state based on the inputs
{
  current_state = CLOSED_STATE;                                       //change the current state to CLOSED_STATE, it's important to do as little as possible during an interrupt.
                                                                      //if you need to do more processing, simply set a flag in the interrupt and let the main loop address it.
  current_attempts = 0;                                               //reset the current_attempts to zero
}

void OPEN_ISR()                                                       //this is the actual ISR (Interrupt Service Routine) where we alter the state based on the inputs
{
  current_state = OPEN_STATE;                                         //change the current state to OPEN_STATE, it's important to do as little as possible during an interrupt.
                                                                      //if you need to do more processing, simply set a flag in the interrupt and let the main loop address it.
  current_attempts = 0;                                               //reset the current_attempts to zero
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////// S E R V O  C O N T R O L   F U N C T I O N S //////////////////////////////////////////////////////////////

PWMServo servo;                                                       //instatiate the PWM Servo Library and give it a variable name of "servo
static int OPEN = 0;                                                  //define the target in degrees that the servo needs to move to for the open state
static int CLOSE = 180;                                               //define the target in degrees that the servo needs to move to for the closed state
static int minPulse = 640;                                            //minimum pulse width in microseconds that the servo can read
static int maxPulse = 2250;                                           //maximum pulse width in microseconds that the servo can read

static uint32_t open_time = 500;                                      //time in milliseconds that it SHOULD take the wing to make this maneuver 
static uint32_t close_time = 500;                                     //time in milliseconds that it SHOULD take the wing to make this maneuver

volatile uint32_t servo_open_start_time = 0;                          //time in milliseconds in which the servo began to open
volatile uint32_t servo_close_start_time = 0;                         //time in milliseconds in which the servo began to close

#define SERVO_PIN 23                                                  //pin on the teensy 3.2 that the signal for the servo is derived from

//this function handles the opening of the control surface
void open_surface()
{
  if (!servo.attached())                                              //if the servo is disabled                 
  {
    servo_open_start_time = millis();                                 //start the open timer
    servo.attach(SERVO_PIN,minPulse,maxPulse);                        //enable the servo
    servo.write(OPEN);                                                //and tell it to open
  }
  
  
}
//this function handles the closing of the control surface
void close_surface()
{
  if (!servo.attached())                                             //if the servo is disabled
  {
    servo_close_start_time = millis();                               //start the close timer
    servo.attach(SERVO_PIN,minPulse,maxPulse);                       //enable the servo
    servo.write(CLOSE);                                              //and tell it to close
  }
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

/////////////////////////////////////////////////////////////////// M A I N  L O O P ///////////////////////////////////////////////////////////////////////////
void loop()
{
  // put your main code here, to run repeatedly:

  target_state = readCAN();//check for CAN messages, set target_state accordingly

  ////////////////////////////////////////////////////////// T A R G E T  I S  O P E N /////////////////////////////////////////////////////////////////////////

  if (target_state == OPEN_STATE)                                  //if we are targeting an open control surface..
  {
    switch (current_state)                                         //decide what were going to do based on the current state
    {
      case OPEN_STATE:                                             //we are already in the OPEN_STATE
        digitalWrite(LED_BUILTIN,HIGH);                            //change the state of the onboard LED, just to debug the system
        servo.detach();
        break;

      case CLOSED_STATE:                                           //we are currently in the CLOSED_STATE
        digitalWrite(LED_BUILTIN,LOW);                             //change the state of the onboard LED, just to debug the system
        open_surface();                                            //send the message to open the control surface
        start_time = millis();                                     //record the start time of the opening process, in ms since the system booted
        current_state = TRANSITIONING_OPEN_STATE;                  //change the state to TRANSITIONING_OPEN_STATE
        break;

      case TRANSITIONING_OPEN_STATE:                               //we are already in the TRANSITIONING_OPEN_STATE
        digitalWrite(LED_BUILTIN,LOW);                             //change the state of the onboard LED, just to debug the system
        if((millis() - start_time) > timeout)                      //if we have exceeded the timeout threshold, and still haven't reached the target state
        {
          current_state = UNKNOWN_STATE;                           //set the state to UNKNOWN_STATE, indicating an error has occurred
        }
        break;

      case TRANSITIONING_CLOSED_STATE:                             //we are already in the TRANSITIONING_CLOSED_STATE
        digitalWrite(LED_BUILTIN,LOW);                             //change the state of the onboard LED, just to debug the system
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
    if (millis() - servo_open_start_time > open_time)             //if the servo open_time has been exceeded
    {
      servo.detach();                                             //disable the servo, this conserves wear and tear on the servo 
    }
  }
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////// T A R G E T  I S  C L O S E D ///////////////////////////////////////////////////////////////////////

  else if (target_state == CLOSED_STATE)                           //if we are targeting a closed control surface..
  {
    switch (current_state)                                         //decide what were going to do based on the current state
    {
      case OPEN_STATE:                                             //we are already in the OPEN_STATE
        digitalWrite(LED_BUILTIN,LOW);                             //change the state of the onboard LED, just to debug the system
        close_surface();                                           //send the message to close the control surface
        start_time = millis();                                     //record the start time of the opening process, in ms since the system booted
        current_state = TRANSITIONING_CLOSED_STATE;                //set the current_state to TRANSITIONING_CLOSED_STATE
        break;

      case CLOSED_STATE:                                           //we are currently in the CLOSED_STATE
        digitalWrite(LED_BUILTIN,HIGH);                            //change the state of the onboard LED, just to debug the system
        servo.detach();
        break;

      case TRANSITIONING_OPEN_STATE:                               //we are already in the TRANSITIONING_OPEN_STATE
        digitalWrite(LED_BUILTIN,LOW);                             //change the state of the onboard LED, just to debug the system
        close_surface();                                           //send the message to close the control surface
        start_time = millis();                                     //record the start time of the opening process, in ms since the system booted
        current_state = TRANSITIONING_CLOSED_STATE;                //set the current_state to TRANSITIONING_CLOSED_STATE
        break;

      case TRANSITIONING_CLOSED_STATE:                             //we are already in the TRANSITIONING_CLOSED_STATE
        digitalWrite(LED_BUILTIN,LOW);                             //change the state of the onboard LED, just to debug the system
        if((millis() - start_time) > timeout)                      //if we have exceeded the timeout threshold, and still haven't reached the target state
        {
          current_state = UNKNOWN_STATE;                           //set the current_state to UNKNOWN_STATE
        }
        break;

      case UNKNOWN_STATE:                                          //we are already in the UNKNOWN_STATE
        digitalWrite(LED_BUILTIN,HIGH);                            //change the state of the onboard LED, just to debug the system
        if (current_attempts < attempts)                                      //if we haven't tried to close the control surface more than "attempts"
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
    if (millis() - servo_close_start_time > close_time)            //if the servo close_time has been exceeded
    {
      servo.detach();                                              //disable the servo, this conserves wear and tear on the servo
    }
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
