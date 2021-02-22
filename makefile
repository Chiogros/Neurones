OBJS	= main.o mind.o neurone.o IHM.o
SOURCE	= main.cpp mind.cpp neurone.cpp IHM.cpp
HEADER	= mind.h neurone.h NUMBER.h IHM.h
OUT	= main
CC	 = g++
FLAGS	 = -g -c -Wall -Wextra
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp -std=c++17

mind.o: mind.cpp
	$(CC) $(FLAGS) mind.cpp -std=c++17

neurone.o: neurone.cpp
	$(CC) $(FLAGS) neurone.cpp -std=c++17

IHM.o: IHM.cpp
	$(CC) $(FLAGS) IHM.cpp -std=c++17

clean:
	rm -f $(OBJS) $(OUT)
