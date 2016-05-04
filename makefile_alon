OBJS = main.o House.o Point.o Sensor.o Reader.o Simulation.o Simulator.o 
CC = g++
DEBUG = -g
CFLAGS = -Wall -c -O2 -std=c++11 -pedantic -pthread $(DEBUG)
LFLAGS = -Wall $(DEBUG)

simulator : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o simulator -ldl
	
Point.o : Point.h Point.cpp
	$(CC) $(CFLAGS) Point.cpp

House.o : House.h House.cpp Point.h
	$(CC) $(CFLAGS) House.cpp

Sensor.o : Sensor.h Sensor.cpp AbstractSensor.h House.h Point.h
	$(CC) $(CFLAGS) Sensor.cpp

Reader.o : Reader.h Reader.cpp House.h AbstractAlgorithm.h
	$(CC) $(CFLAGS) Reader.cpp

Simulation.o : Simulation.h Simulation.cpp Direction.h AbstractSensor.h AbstractAlgorithm.h Point.h House.h
	$(CC) $(CFLAGS) Simulation.cpp

Simulator.o : Simulator.cpp Simulator.cpp AbstractSensor.h AbstractAlgorithm.h House.h Simulation.h
	$(CC) $(CFLAGS) Simulator.cpp

main.o : Simulator.h Simulation.h AbstractAlgorithm.h House.h AlgorithmFactory.h Reader.h
	$(CC) $(CFLAGS) main.cpp -ldl

clean:
	\rm *.o simulator
