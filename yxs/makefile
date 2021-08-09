INC=
LIB= -pthread
CC=g++
CC_FLAG= -g -Wall -std=c++11

PRG=tcpclient
OBJ=main.o tcpmsg.o  clientmgr.o
	
$(PRG):$(OBJ)
	$(CC) $(INC) $(LIB) -o $@ $(OBJ)

.SUFFIXES: .c .o .cpp
.cpp.o:
	$(CC) $(CC_FLAG) $(INC) -c $*.cpp -o $*.o

.PRONY:clean
clean:
	@echo "Removing linked and compiled files......"
	rm -f $(OBJ) $(PRG)