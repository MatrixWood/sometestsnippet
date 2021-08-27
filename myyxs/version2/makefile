CXXFLAGS = -O0 -g -Wall -I . -pthread
LDFLAGS = -lpthread
LIB_SRC = 
BASE_SRC = InetAddress.cpp Socket.cpp SocketsOps.cpp TcpClient.cpp ConfigReader.cpp utils/Log.cpp
BINARIES = YxsGame

all: $(BINARIES)

$(BINARIES):
	g++ $(CXXFLAGS) -o $@ $(LIB_SRC) $(BASE_SRC) $(filter %.cpp,$^) $(LDFLAGS)

clean:
	rm -f $(BINARIES) core

YxsGame: YxsGame.cpp