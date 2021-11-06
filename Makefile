#MAKE FILE
#Project 1: Pair Wars
#Completed by: Jacob Chrisitan (jbc160) and Levi Jacoby (ldj52)

OBJS	= DistributionGenerator.o Driver.o Event.o Process2.o main.o Process.o
SOURCE	= main.cpp DistributionGenerator.cpp Driver.cpp Event.cpp Process2.cpp Process.cpp
HEADER	= DistributionGenerator.h Driver.h Event.h Process2.h Process.h
OUT	= main
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

Process2.o: Process2.cpp
	$(CC) $(FLAGS) Process2.cpp

Process.o: Process.cpp
	$(CC) $(FLAGS) Process.cpp

clean:
	rm -f $(OBJS)

run : $(OUT)
	./$(OUT)

.PHONY: clean