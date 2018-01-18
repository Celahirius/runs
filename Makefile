PROJ = ./
INCDIR = /usr/local/include
LIBDIR = /usr/local/lib
EXEC_FILE = runs


all:
	clear
	g++ -c main.cpp -I $(INCDIR) -o main.o -std=c++11
	g++ main.o -L $(LIBDIR) -lpqxx -lpq -o $(EXEC_FILE) -std=c++11

run:
	clear
	g++ -c main.cpp -I $(INCDIR) -o main.o -std=c++11
	g++ main.o -L $(LIBDIR) -lpqxx -lpq -o $(EXEC_FILE) -std=c++11
	clear
	./$(EXEC_FILE)
