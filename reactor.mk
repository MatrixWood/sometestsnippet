CXXFLAGS = -O0 -g -Wall -I base/ -pthread
LDFLAGS = -lpthread
BASE_SRC = ../base/Thread.cpp

$(BINARIES):
	g++ $(CXXFLAGS) -o $@ $(LIB_SRC) $(BASE_SRC) $(filter %.cpp,$^) $(LDFLAGS)

clean:
	rm -f $(BINARIES) core