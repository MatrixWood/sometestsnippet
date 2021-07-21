CXXFLAGS = -O0 -g -Wall -I . -pthread
LDFLAGS = -lpthread
LIB_SRC = Channel.cpp EventLoop.cpp Poller.cpp Thread.cpp TimerQueue.cpp Timestamp.cpp 
BASE_SRC = 
BINARIES = main

all: $(BINARIES)

$(BINARIES):
	g++ $(CXXFLAGS) -o $@ $(LIB_SRC) $(BASE_SRC) $(filter %.cpp,$^) $(LDFLAGS)

clean:
	rm -f $(BINARIES) core

main: main.cpp