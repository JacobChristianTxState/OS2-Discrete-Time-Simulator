OBJS	= DistributionGenerator.o Driver.o Event.o main.o Process.o SimulatorLogger.o StatisticDriver.o
SOURCE	= main.cpp DistributionGenerator.cpp Driver.cpp Event.cpp Process.cpp SimulatorLogger.cpp StatisticDriver.cpp
HEADER	= DistributionGenerator.h Driver.h Event.h Process.h SimulatorLogger.h StatisticDriver.h EventTypeEnum.h
OUT	= main.exe
CC	 = g++
FLAGS	 = -g -c -Wall -std=c++0x
LFLAGS	 = 

all: main clean

main: $(OBJS) 
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS) 

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp 

DistributionGenerator.o: DistributionGenerator.cpp
	$(CC) $(FLAGS) DistributionGenerator.cpp 

Driver.o: Driver.cpp
	$(CC) $(FLAGS) Driver.cpp 

Event.o: Event.cpp
	$(CC) $(FLAGS) Event.cpp 

SimulatorLogger.o: SimulatorLogger.cpp
	$(CC) $(FLAGS) SimulatorLogger.cpp

Process.o: Process.cpp
	$(CC) $(FLAGS) Process.cpp

StatisticDriver.o: StatisticDriver.cpp
	$(CC) $(FLAGS) StatisticDriver.cpp

clean:
	rm -f $(OBJS)

run : $(OUT)
	./$(OUT)

.PHONY: clean