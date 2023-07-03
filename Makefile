CC = g++
CFLAGS = -std=c++11 -I/path/to/graphviz/include
LDFLAGS = -L/path/to/graphviz/lib
LDLIBS = -lcgraph

SOURCES = toposort.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = toposort

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE) $(LDLIBS)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
