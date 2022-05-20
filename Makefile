CC=g++
CXXFLAGS=-c -std=c++2a
LDFLAGS=-lodbc -pthread
SRCS= src/DataGen.cpp src/DataSrc.cpp src/UserInput.cpp src/Driver.cpp \
src/DBInit.cpp src/SQLDialect.cpp src/Barrier.cpp src/AnalyticalClient.cpp \
src/TransactionalClient.cpp src/Workload.cpp src/Globals.cpp \
src/GetFromDB.cpp src/HATtrickBench.cpp src/Results.cpp src/Frontier.cpp src/LinkedList.cpp
OBJECTS=$(SRCS:.cpp=.o)
EXECUTABLE=HATtrickBench

all: $(SRCS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CXXFLAGS) $< -o $@

clean:
	rm HATtrickBench
	rm src/*.o
	rm -rf results
