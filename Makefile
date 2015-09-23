CC=g++
CFLAGS=-O3 -fpermissive -O3 -g -c
LDFLAGS=
SOURCES=$(shell find | grep .cpp | cut -c 3- | tr '\n' ' ')
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=bin/raycast

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ -lSDL2

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(shell find . -name "*.o" -type f|xargs rm -f)
