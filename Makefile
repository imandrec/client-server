CC = g++
CXX_FLAGS := -w -Wall -Wextra -std=c++17 -ggdb #Flag use to detect memory leack
GDB = -g # flag for the debuging
REMOVE = -rf # Flag for removing the files
LIB = -pthread # use to include the pthread library

IP = 127.0.0.1
INPUT_FILE = client_config_1.txt 

all: # execute the file 
	$(CC) $(GDB) $(CXX_FLAGS) $(LIB) Client.cpp -o Client
	$(CC) $(GDB) $(CXX_FLAGS) $(LIB) Server.cpp -o Server
	
Server_exe:
	clear
	./Server 5000

Client_exe:
	clear
	./Client $(IP) $(INPUT_FILE)

clean_exe:
	rm $(REMOVE) Client
	rm $(REMOVE) Server
	
