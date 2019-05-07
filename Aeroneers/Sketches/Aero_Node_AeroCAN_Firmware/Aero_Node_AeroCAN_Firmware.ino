#include<AeroCAN.h>

AeroCANnode node((uint8_t) 2);

CAN_filter_t extendedFilter;

void openDebounceInterrupt()
{
  node.openDebounceInterrupt();
}
void closeDebounceInterrupt()
{
  node.closeDebounceInterrupt();
}

////////////////////////////////////////////////////// A R D U I N O  S E T U P  F U N C T I O N ////////////////////////////////////////////////////////////////
void setup()
{
  // put your setup code here, to run once:
  Can0.begin(1000000);
  //extendedFilter.id = 0;
  //extendedFilter.ext = 1;
  //extendedFilter.rtr = 0;
  //Can0.setFilter(extendedFilter,0);

  pinMode(LED_BUILTIN,OUTPUT);                      //declare the LED as an output
  attachInterrupt(7,openDebounceInterrupt,RISING);  //attach the openDebounceInterrupt function to the rising edge of input on pin 7
  attachInterrupt(8,closeDebounceInterrupt,RISING); //attach the closeDebounceInterrupt function to the rising edge of input on pin 8
  Serial.begin(115200);                             //Start the Serial terminal, this is for debugging only.
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////// M A I N  L O O P ///////////////////////////////////////////////////////////////////////////
void loop()
{
  node.run();

}
