CC=g++
CFLAGS=-O3 -fpermissive -O3 -g -c -I/usr/local/include
LDFLAGS=-L/usr/local/lib
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=bin/raycast

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ -lSDL2

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) *.o
	$(RM) $(EXECUTABLE)
