#   Makefile for MPU9250 WiringPi examples
#
#   Copyright (C) 2018 Simon D. Levy
#
#   This file is part of MPU.
#
#   MPU is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   MPU is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#   You should have received a copy of the GNU General Public License
#   along with MPU.  If not, see <http://www.gnu.org/licenses/>.


####### Cross Platform Data Bus Directories ##########
CPDB   = ./libraries/CrossPlatformDataBus-master
CPSRC  = $(CPDB)/src
CPEXT  = $(CPDB)/extras
CPCMN  = $(CPEXT)/common
I2CSRC = $(CPEXT)/i2c/wiringpi/src
######################################################

############# MPU Library Directories ################
MPUSRC = ./libraries/MPU-master/src
######################################################

ALL = MPU_test


all: $(ALL)
################################### E V E R Y T H I N G  B E L O W  I S  F O R  T H E  M P U  L I B R A R Y ####################
MPU_test: MPU_test.o MPU.o MPU9250.o MPU9250_Master.o MPU9250_Master_I2C.o WiringPiI2C.o errmsg.o
	g++ -std=c++11 -o MPU_test MPU_test.o MPU.o MPU9250.o MPU9250_Master.o MPU9250_Master_I2C.o WiringPiI2C.o errmsg.o -lwiringPi

MPU_test.o: MPU_test.cpp
	g++ -std=c++11 -Wall -I$(CPCMN) -I$(MPUSRC) -c MPU_test.cpp

MPU.o: $(MPUSRC)/MPU.cpp
	g++ -std=c++11 -Wall -I$(MPUSRC) -c $(MPUSRC)/MPU.cpp

MPU9250.o: $(MPUSRC)/MPU9250.cpp
	g++ -std=c++11 -Wall -I$(MPUSRC) -c $(MPUSRC)/MPU9250.cpp

MPU9250_Master_I2C.o: $(MPUSRC)/MPU9250_Master_I2C.cpp
	g++ -std=c++11 -Wall -I$(CPSRC) -I$(MPUSRC) -c $(MPUSRC)/MPU9250_Master_I2C.cpp

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
