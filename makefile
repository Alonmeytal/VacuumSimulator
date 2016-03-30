OBJS = House.o Point.o Sensor.o Settings.o SimpleAlgorithm.o Simulation.o simulator.o 
CC = g++
DEBUG = -g
CFLAGS = -Wall -c -O2 -std=c++11 -pedantic -pthread $(DEBUG)
LFLAGS = -Wall $(DEBUG)

simulator : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o simulator

Point.o : Point.h Point.cpp
	$(CC) $(CFLAGS) Point.cpp

House.o : House.h House.cpp Point.h
	$(CC) $(CFLAGS) House.cpp

Sensor.o : Sensor.h Sensor.cpp AbstractSensor.h House.h Point.h
	$(CC) $(CFLAGS) Sensor.cpp

Settings.o : Settings.h Settings.cpp
	$(CC) $(CFLAGS) Settings.cpp

SimpleAlgorithm.o : SimpleAlgorithm.cpp Direction.h AbstractAlgorithm.h Sensor.h
	$(CC) $(CFLAGS) SimpleAlgorithm.cpp

Simulation.o : Simulation.h Simulation.cpp Direction.h AbstractSensor.h AbstractAlgorithm.h Point.h
	$(CC) $(CFLAGS) Simulation.cpp

simulator.o : simulator.cpp Direction.h SensorInformation.h AbstractSensor.h AbstractAlgorithm.h House.h Settings.h HouseReader.h Score.h
	$(CC) $(CFLAGS) simulator.cpp

clean:
	\rm *.o simulator
