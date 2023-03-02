CC = g++
CFLAGS  = -O3 -fopenmp -std=c++17


default: action
agents.o: Agents.cpp Agents.h
	$(CC) $(CFLAGS) -c Agents.cpp 
NK_space_action-c8.o: NK_space_action-c8.cpp
	$(CC) $(CFLAGS) -c NK_space_action-c8.cpp
NK_space_action-c8_weighted.o: NK_space_action-c8_weighted.cpp
	$(CC) $(CFLAGS) -c NK_space_action-c8_weighted.cpp
action: Agents.o NK_space_action-c8.o
	$(CC) $(CFLAGS) -o action Agents.o NK_space_action-c8.o
actionw: Agents.o NK_space_action-c8_weighted.o
	$(CC) $(CFLAGS) -o actionw NK_space_action-c8_weighted.o Agents.o
agents4.o: Agents4.cpp Agents4.h
	$(CC) $(CFLAGS) -c Agents4.cpp
NK_space_action-c4.o: NK_space_action-c4.cpp
	$(CC) $(CFLAGS) -c NK_space_action-c4.cpp
action4: Agents4.o NK_space_action-c4.o
	$(CC) $(CFLAGS) -o action4 Agents4.o NK_space_action-c4.o

Agents6.o: Agents6.cpp Agents6.h
	$(CC) $(CFLAGS)  -c Agents6.cpp 

NK_space_action-c6.o: NK_space_action-c6.cpp
	$(CC) $(CFLAGS) -c NK_space_action-c6.cpp

action6: Agents6.o NK_space_action-c6.o
	$(CC) $(CFLAGS) -o action6 Agents6.o NK_space_action-c6.o


clean:
	rm ./*.o
all: action actionw
