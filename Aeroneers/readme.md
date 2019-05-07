## AeroCAN
![
](https://static1.squarespace.com/static/5c5a36a594d71ab0f5320f36/t/5cd09ff9104c7b2f4c380228/1557176321576/IMG_1575.jpg?format=2500w)
AeroCAN is all about removing the attention-load of the race car driver and enhancing a racers ability to get around track. It accomplishes this by reading in data from an onboard accelerometer, processing the signal with a digital signal processing algorithm, and controlling the action of control surfaces mounted on a motor vehicle.

## The System:

The system is logically split into three layers:

**The Data Acquisition Layer:**

This layer is responsible for reading the raw data from an IMU or Inertial Measurement Unit and applies a digital filter to the signal to reduce unwanted noise and anomalies.

**The Node Control Layer:**

This layer is responsible for receiving commands from the main control layer and enforcing those commands on physical control surfaces arranged around the car. It determines the position of the control surfaces by means of limit switches mounted on the car.

**The Main Control Layer:**

This layer is responsible for interpreting the data from the acquisition layer and orchestrating the control of the nodes in order to make the cars aero package, that is all control surfaces working together, function to the benefit of the driver.

# The Hardware:

From the outset, we chose to stick with components that were sufficient enough to do the job, but not excessively expensive such that the end result would never be a viable product.

With this in mind we chose the following components:

## **Node Board:**

-   Teensy 3.2 Arduino-compatible 32bit ARM Cortex M4 based microcontroller
    
-   Texas Instruments SN65HVD230 3.3V CAN Transceiver
    
-   Optically isolated digital inputs for control surface feedback
    
-   On board power regulation for protection against voltage fluctuations
    
![enter image description here](https://static1.squarespace.com/static/5c5a36a594d71ab0f5320f36/t/5cb5009af4e1fc48ee9b6618/1555366063471/?format=1500w)

## **Main Board:**

   Teensy 3.6 Arduino-compatible 32bit ARM Cortex M4 based microcontroller
    
-   Texas Instruments SN65HVD230 3.3V CAN Transceiver
    
-   Optically isolated digital inputs for human control feedback
    
-   On board power regulation for protection against voltage fluctuations
    
-   MPU9250 9-axis IMU
    
![enter image description here](https://static1.squarespace.com/static/5c5a36a594d71ab0f5320f36/t/5cb50074e79c70a0145047d1/1555366016661/mainboard?format=750w)

# The Software

**CAN Application Layer Protocol**

Rather than using the 11 bit CAN identifier to address each node individually, we decided a protocol is necessary to spread all of the functionality across the address space to facilitate ease of use and clarity of messages on the bus.

We achieved this by segmenting the 11 bit address space into two sections, the upper 5 bits of the address are reserved for “opcodes” so to speak, and the lower 6 bits are reserved for the address which an opcode is to act upon.

See the application protocol document for more reference: 

[AeroCAN Application Protocol](https://github.com/brandonhanner/SeniorDesign/blob/master/Aeroneers/Aeroneers%20CAN%20Protocol.pdf)

**AeroCAN Library**
The AeroCAN library was written to facilitate the implementation of an active aero system with different control theories. Our implementation is based solely off of the use of an accelerometer but by using the Teensy 3.6m you have access to a secondary CANbus which could open the door for using things like throttle, brake, steering angle, or any other operating parameters that might be available on a motorsports vehicle that could help teams perfect their control algorithm. This library lets users focus on control theory and leaves the operations of communication, control surface control, and configuration up to the library. 

While AeroCAN was developed on Teensy hardware and the FlexCAN library, it was designed such that the routines for CANcommunication could be adapted to specific hardware with relative ease. 

[Check out the AeroCAN Library Here]
(https://github.com/brandonhanner/SeniorDesign/tree/master/Aeroneers/libraries/AeroCAN)

# Demo Setup
Being a senior design project, we had to demo our project in the gym of the university. Since cars are not permitted in the gym, we decided to call in the help of Forza Motorsport 7's data out feature to send telemetry from the games physics engine to a wifi enabled microcontroller with a CAN peripheral to act as the main control board. Telemetry is sent via a 312 byte UDP packet containing over 70 parameters of telemetry, which is then parsed by the micro controller and the acceleration vectors are extracted and fed into the main layer to decide how the control surfaces should react. Attached via CANbus is three nodes controlling 3 servos with a aero control surface (model wing) attached to it. This is accomplished using the ForzaUDP2CANnoAeroCAN sketch on an ESP32 with the same CAN transceiver used in the final product.


![enter image description here](https://static1.squarespace.com/static/5c5a36a594d71ab0f5320f36/t/5cd1f60142c6ac00012e115e/1557263887685/IMG_1578.jpg?format=2500w)


[Demo Video of AeroCAN](https://youtu.be/JjrttWb5x9Q)
