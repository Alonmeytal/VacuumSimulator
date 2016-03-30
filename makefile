OBJS = House.o Point.o Sensor.o Settings.o SimpleAlgorithm.o Simulation.o simulator.o 
CC = g++
DEBUG = -g
CFLAGS = -Wall -c -O2 -std=c++11 -pedantic -pthread $(DEBUG)
LFLAGS = -Wall $(DEBUG)

simulator : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o simulator

Point.o : include/Point.h Point.cpp
	$(CC) $(CFLAGS) Point.cpp

House.o : include/House.h House.cpp include/Point.h
	$(CC) $(CFLAGS) House.cpp

Sensor.o : include/Sensor.h Sensor.cpp include/AbstractSensor.h include/House.h includ/Point.h
	$(CC) $(CFLAGS) Sensor.cpp

Settings.o : include/Settings.h Settings.cpp
	$(CC) $(CFLAGS) Settings.cpp

	SimulationOnHouse.o : include/SimulationOnHouse.h SimulationOnHouse.cpp include/Direction.h include/Coordinate.h include/House.h include/SensorInformation.h include/SimulationSensor.h include/RealTimeInfo.h include/AlgorithmSimulation.h
		$(CC) $(CFLAGS) SimulationOnHouse.cpp
		
	SimulationSensor.o : include/SimulationSensor.h SimulationSensor.cpp include/AbstractSensor.h include/SensorInformation.h
		$(CC) $(CFLAGS) SimulationSensor.cpp

SimpleAlgorithm.o : SimpleAlgorithm.cpp include/Direction.h include/AbstractAlgorithm.h include/Sensor.h
	$(CC) $(CFLAGS) SimpleAlgorithm.cpp

Simulation.o : include/Simulation.h Simulation.cpp include/Direction.h include/AbstractSensor.h include/AbstractAlgorithm.h include/Point.h
	$(CC) $(CFLAGS) Simulation.cpp

simulator.o : simulator.cpp include/Direction.h include/SensorInformation.h include/AbstractSensor.h include/AbstractAlgorith.h include/House.h include/RealTimeInfo.h include/SimpleAlgorithm.h include/Settings.h include/HouseReader.h include/Score.h
	$(CC) $(CFLAGS) simulator.cpp

clean:
	\rm *.o simulator
