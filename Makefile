all: main.o
	gcc -o werhauz main.o -g

main.o: main.c
	gcc -g -c main.c