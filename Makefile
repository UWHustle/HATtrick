CC=g++

CXXFLAGS=-c -std=c++2a -Wall -Werror \
	-g -O2 -DNDEBUG # release mode
LDFLAGS=-lodbc -pthread
SRCS= src/DataGen.cpp src/DataSrc.cpp src/UserInput.cpp src/Driver.cpp \
	src/DBInit.cpp src/SQLDialect.cpp src/Barrier.cpp src/AnalyticalClient.cpp \
	src/TransactionalClient.cpp src/Workload.cpp src/Globals.cpp \
	src/GetFromDB.cpp src/HATtrickBench.cpp src/Results.cpp src/Frontier.cpp src/LinkedList.cpp

INC=
LIB=

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	INC += -I$(shell brew --prefix unixodbc)/include
	LIB += -L$(shell brew --prefix unixodbc)/lib
endif

OBJECTS=$(SRCS:.cpp=.o)

EXECUTABLE=HATtrickBench

all: $(SRCS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIB) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CXXFLAGS) $(INC) $< -o $@

.PHONY: clean
clean:
	rm $(OBJECTS) $(EXECUTABLE)
	rm -rf results
