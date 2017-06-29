build : main.o Engine.o
	g++ -o snake main.o Engine.o -lncurses -lpthread
