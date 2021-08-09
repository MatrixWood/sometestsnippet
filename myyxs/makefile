CXXFLAGS = -O0 -g -Wall -I . -pthread
LDFLAGS = -lpthread
LIB_SRC = 
BASE_SRC = 
BINARIES = yxsclient

all: $(BINARIES)

$(BINARIES):
	g++ $(CXXFLAGS) -o $@ $(LIB_SRC) $(BASE_SRC) $(filter %.cpp,$^) $(LDFLAGS)

clean:
	rm -f $(BINARIES) core

yxsclient: yxsclient.cpp