all: main.o insert.o
	gcc -o werhauz main.o insert.o -g

main.o: main.c
	gcc -g -c main.c

insert.o: insert.c
	gcc -g -c insert.c