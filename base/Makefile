CXXFLAGS=-g -Wall -I /usr/local/include
CXXFLAGS+=-O2

HEADERS=$(wildcard *.h)
TESTS= atomic_unittest \
	thread_test

all: $(TESTS)

$(TESTS): $(HEADERS)

$(TESTS):
	$(CXX) $(CXXFLAGS) -o $@ $(filter %.cpp,$^) $(LDFLAGS) -lpthread

atomic_unittest: test/Atomic_unittest.cpp

thread_test: test/Thread_test.cpp Thread.cpp

clean:
	rm -f $(TESTS)