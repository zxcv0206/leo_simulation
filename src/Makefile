CC=g++
CFLAGS=-Wall -Wextra -O0 -g -Wall

dependency= Data.o Node.o Link.o AlgorithmBase.o MyAlgo.o debug.o

a.out: main.cpp $(dependency)
	$(CC) $(CFLAGS) main.cpp $(dependency)

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c $<

clean:
	-rm *.out
	-rm *.o