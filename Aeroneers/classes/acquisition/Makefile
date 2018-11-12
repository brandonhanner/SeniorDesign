#Makefile for the acquisition layer stuff

####### Cross Platform Data Bus Directories ##########
CPDB   = ../../libraries/CrossPlatformDataBus-master
CPSRC  = $(CPDB)/src
CPEXT  = $(CPDB)/extras
CPCMN  = $(CPEXT)/common
I2CSRC = $(CPEXT)/i2c/wiringpi/src
######################################################

############# MPU Library Directories ################
MPUSRC = ../../libraries/MPU-master/src
######################################################

ALL = MPU_test IMU_test filter_test timertest


all: $(ALL)

filter_test: filter_test.o IMU.o MPU.o MPU9250.o MPU9250_Master.o MPU9250_Passthru.o WiringPiI2C.o errmsg.o
	g++ -std=c++11 -o filter_test filter_test.o IMU.o MPU.o MPU9250.o MPU9250_Master.o MPU9250_Passthru.o WiringPiI2C.o errmsg.o -lwiringPi

filter_test.o:filter_test.cpp
	g++ -std=c++11 -Wall -I$(CPCMN) -I$(MPUSRC) -c filter_test.cpp

timertest: timertest.o IMU.o MPU.o MPU9250.o MPU9250_Master.o MPU9250_Passthru.o WiringPiI2C.o errmsg.o
	g++ -std=c++11 -o timertest timertest.o IMU.o MPU.o MPU9250.o MPU9250_Master.o MPU9250_Passthru.o WiringPiI2C.o errmsg.o -lwiringPi

timertest.o:timertest.cpp
	g++ -std=c++11 -Wall -I$(CPCMN) -I$(MPUSRC) -c timertest.cpp

IMU_test: IMU_test.o IMU.o MPU.o MPU9250.o MPU9250_Master.o MPU9250_Passthru.o WiringPiI2C.o errmsg.o
	g++ -std=c++11 -o IMU_test IMU_test.o IMU.o MPU.o MPU9250.o MPU9250_Master.o MPU9250_Passthru.o WiringPiI2C.o errmsg.o -lwiringPi

IMU_test.o: IMU_test.cpp
	g++ -std=c++11 -Wall -I$(CPCMN) -I$(MPUSRC) -c IMU_test.cpp

IMU.o:
	g++ -std=c++11 -Wall -I$(CPCMN) -I$(MPUSRC) -c IMU.cpp

MPU_test: MPU_test.o MPU.o MPU9250.o MPU9250_Master.o MPU9250_Master_I2C.o WiringPiI2C.o errmsg.o
	g++ -std=c++11 -o MPU_test MPU_test.o MPU.o MPU9250.o MPU9250_Master.o MPU9250_Master_I2C.o WiringPiI2C.o errmsg.o -lwiringPi

MPU_test.o: MPU_test.cpp
	g++ -std=c++11 -Wall -I$(CPCMN) -I$(MPUSRC) -c MPU_test.cpp
################################### E V E R Y T H I N G  B E L O W  I S  F O R  T H E  M P U  L I B R A R Y ####################
MPU.o: $(MPUSRC)/MPU.cpp
	g++ -std=c++11 -Wall -I$(MPUSRC) -c $(MPUSRC)/MPU.cpp

MPU9250.o: $(MPUSRC)/MPU9250.cpp
	g++ -std=c++11 -Wall -I$(MPUSRC) -c $(MPUSRC)/MPU9250.cpp

MPU9250_Master_I2C.o: $(MPUSRC)/MPU9250_Master_I2C.cpp
	g++ -std=c++11 -Wall -I$(CPSRC) -I$(MPUSRC) -c $(MPUSRC)/MPU9250_Master_I2C.cpp

MPU9250_Passthru.o: $(MPUSRC)/MPU9250_Passthru.cpp
	g++ -std=c++11 -Wall -I$(CPSRC) -I$(MPUSRC) -c $(MPUSRC)/MPU9250_Passthru.cpp

MPU9250_Master.o: $(MPUSRC)/MPU9250_Master.cpp
	g++ -std=c++11 -Wall -I$(MPUSRC) -c $(MPUSRC)/MPU9250_Master.cpp

WiringPiI2C.o: $(I2CSRC)/WiringPiI2C.cpp
	g++ -std=c++11 -Wall -I$(CPSRC) -c $(I2CSRC)/WiringPiI2C.cpp

errmsg.o: $(CPCMN)/errmsg.cpp
	g++ -std=c++11 -Wall -c $(CPCMN)/errmsg.cpp
################################### E V E R Y T H I N G  A B O V E  I S  F O R  T H E  M P U  L I B R A R Y ####################
run: MPU_test
	./MPU_test

edit:
	vim MPU_test.cpp

clean:
	rm -f $(ALL) *.o *~
