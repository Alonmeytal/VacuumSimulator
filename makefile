.DEFAULT_GOAL := all
OBJS = main.o House.o Point.o Sensor.o Reader.o Simulation.o Simulator.o AlgorithmRegistrar.o AlgorithmRegistration.o Errorton.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c -O2 -std=c++14 -pedantic -pthread $(DEBUG)
LFLAGS = -Wall $(DEBUG)

simulator : $(OBJS)
	$(CC) -rdynamic $(LFLAGS) $(OBJS) -o simulator -ldl

all: simulator _305008864_A_.so _305008864_B_.so _305008864_C_.so
	
Point.o : Point.h Point.cpp
	$(CC) $(CFLAGS) Point.cpp

House.o : House.h House.cpp Point.h
	$(CC) $(CFLAGS) House.cpp

Sensor.o : Sensor.h Sensor.cpp AbstractSensor.h House.h Point.h
	$(CC) $(CFLAGS) Sensor.cpp

Reader.o : Reader.h Reader.cpp House.h AbstractAlgorithm.h Errorton.h
	$(CC) $(CFLAGS) Reader.cpp

Simulation.o : Simulation.h Simulation.cpp Direction.h AbstractSensor.h AbstractAlgorithm.h Point.h House.h
	$(CC) $(CFLAGS) Simulation.cpp

Simulator.o : Simulator.cpp Simulator.cpp AbstractSensor.h AbstractAlgorithm.h House.h Simulation.h
	$(CC) $(CFLAGS) Simulator.cpp

AlgorithmRegistrar.o : AlgorithmRegistrar.cpp AlgorithmRegistrar.h AlgorithmRegistration.h AbstractAlgorithm.h
	$(CC) $(CFLAGS) AlgorithmRegistrar.cpp

AlgorithmRegistration.o : AlgorithmRegistration.cpp AlgorithmRegistrar.h AbstractAlgorithm.h
	$(CC) $(CFLAGS) AlgorithmRegistration.cpp

Errorton.o : Errorton.cpp
	$(CC) $(CFLAGS) Errorton.cpp

main.o : Simulator.h Simulation.h AbstractAlgorithm.h House.h Reader.h AlgorithmRegistrar.h Errorton.h
	$(CC) $(CFLAGS) main.cpp -ldl

_305008864_A.o: _305008864_A.cpp _305008864_A.h AbstractAlgorithm.h Direction.h AbstractSensor.h SensorInformation.h AlgorithmRegistration.h
	$(CC) $(CFLAGS) -Warray-bounds _305008864_A.cpp -mcmodel=large

_305008864_A_.so: _305008864_A.o
	g++ -shared -Wl,-soname,_305008864_A_.so -o _305008864_A_.so _305008864_A.o

_305008864_B.o: _305008864_B.cpp _305008864_B.h AbstractAlgorithm.h Direction.h AbstractSensor.h SensorInformation.h AlgorithmFactory.h
	$(CC) $(CFLAGS) -Warray-bounds _305008864_B.cpp -mcmodel=large

_305008864_B_.so: _305008864_B.o
	g++ -shared -Wl,-soname,_305008864_B_.so -o _305008864_B_.so _305008864_B.o

_305008864_C.o: _305008864_C.cpp _305008864_C.h AbstractAlgorithm.h Direction.h AbstractSensor.h SensorInformation.h AlgorithmFactory.h
	$(CC) $(CFLAGS) -Warray-bounds _305008864_C.cpp -mcmodel=large

_305008864_C_.so: _305008864_C.o
	g++ -shared -Wl,-soname,_305008864_C_.so -o _305008864_C_.so _305008864_C.o



clean:
	\rm *.o simulator
