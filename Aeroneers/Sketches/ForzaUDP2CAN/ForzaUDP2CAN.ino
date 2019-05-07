#include <WiFi.h>
#include <WiFiUdp.h>
#include <cstdint>
#include <math.h>
#include <Filters.h>
#include<CAN_config.h>


CAN_device_t CAN_cfg;               // CAN Config


#include <AeroCANesp.h>

/* WiFi network name and password */
const char * ssid = "portable";
const char * pwd = "00000000";

// IP address to send UDP data to.
// it can be ip address of the server or 
// a network broadcast address
// here is broadcast address
const char * udpAddress = "192.168.1.106";
const int udpPort = 44444;
char incomingPacket[312];

//create UDP instance
WiFiUDP udp;

struct forzaPacket 
{
  int32_t isRaceOn;
  uint32_t timestampMS;
  float engineMaxRPM;
  float engineIdleRPM;
  float engineRPM;
  
  float accelX;
  float accelY;
  float accelZ;
  
  float velocityX;
  float velocityY;
  float veloctiyZ;
  
  float angvelX;
  float angvelY;
  float angvelZ;
  
  float yaw;
  float pitch;
  float roll;
  
  float normalizedSuspensionTravelFrontLeft;
  float normalizedSuspensionTravelFrontRight;
  float normalizedSuspensionTravelRearLeft;
  float normalizedSuspensionTravelRearRight;
  
  float tireSlipRatioFrontLeft;
  float tireSlipRatioFrontRight;
  float tireSlipRatioRearLeft;
  float tireSlipRatioRearRight;
  
  float wheelRotationSpeedFrontLeft;
  float wheelRotationSpeedFrontRight;
  float wheelRotationSpeedRearLeft;
  float wheelRotationSpeedRearRight;
  
  int32_t wheelOnRumbleStripFrontLeft;
  int32_t wheelOnRumbleStripFrontRight;
  int32_t wheelOnRumbleStripRearLeft;
  int32_t wheelOnRumbleStripRearRight;
  
  float wheelPuddleDepthFrontLeft;
  float wheelPuddleDepthFrontRight;
  float wheelPuddleDepthRearLeft;
  float wheelPuddleDepthRearRight;
  
  float surfaceRumbleFrontLeft;
  float surfaceRumbleFrontRight;
  float surfaceRumbleRearLeft;
  float surfaceRumbleRearRight;
  
  float tireSlipAngleFrontLeft;
  float tireSlipAngleFrontRight;
  float tireSlipAngleRearLeft;
  float tireSlipAngleRearRight;
  
  float tireCombinedSlipFrontLeft;
  float tireCombinedSlipFrontRight;
  float tireCombinedSlipRearLeft;
  float tireCombinedSlipRearRight;

  float suspensionTravelMetersFrontLeft;
  float suspensionTravelMetersFrontRight;
  float suspensionTravelMetersRearLeft;
  float suspensionTravelMetersRearRight;

  int32_t carOrdinal;
  int32_t carClass;
  int32_t carPerformanceIndex;
  int32_t drivetrainType;
  int32_t numCyclinders;

  float positionX;
  float positionY;
  float positionZ;

  float speed;
  float power;
  float torque;

  float tireTempFrontLeft;
  float tireTempFrontRight;
  float tireTempRearLeft;
  float tireTempRearRight;

  float boost;
  float fuel;
  float distanceTraveled;
  float bestLap;
  float lastLap;
  float currentLap;
  float currentRaceTime;

  uint16_t lapNumber;
  uint8_t racePosition;

  uint8_t accelPedal;
  uint8_t brakePedal;
  uint8_t clutchPedal;
  uint8_t handbrakePedal;
  uint8_t gear;
  int8_t steer;
  int8_t normalizedDrivingLine;
  int8_t normalizedAIBrakeDifference;
};

forzaPacket forza;

FilterOnePole lowpassFilterX( LOWPASS, 15.0 );

AeroCANcontroller controller;

void setup(){
  Serial.begin(115200);
  
  //Connect to the WiFi network
   WiFi.begin(ssid, pwd);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  //This initializes udp and transfer buffer
  udp.begin(udpPort);
  pinMode(LED_BUILTIN,OUTPUT);
  
}

void loop(){

int packetSize = udp.parsePacket();
if (packetSize)
{
  int len = udp.read(incomingPacket, 312);
  memcpy(&forza,&incomingPacket,312);
  float acceleration = sqrt((forza.accelX*forza.accelX) + (forza.accelZ*forza.accelZ));
  float angle = round( atan2 (forza.accelZ, forza.accelX) * 180/3.14159265 );
  if( angle < 0){
    angle += 360;
  }
  lowpassFilterX.input( acceleration );
  acceleration /= 9.81;
  Serial.println(acceleration);
  
  if (acceleration > 1.0 && (angle>110 || angle < 70))
  {
    
    digitalWrite(LED_BUILTIN,HIGH);
    controller.setTargetGroup(1,CLOSED_STATE);
  }
  else if (acceleration < .5 || (angle <110 && angle>70))
  {
   
    digitalWrite(LED_BUILTIN,LOW);
    controller.setTargetGroup(1,OPEN_STATE);
  }
}
else 
{
  //Serial.println("No packet");
}
delay(5);
}
