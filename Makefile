all: main.o insert.o delete.o
	gcc -o werhauz main.o insert.o delete.o -g

main.o: main.c
	gcc -g -c main.c

insert.o: insert.c
	gcc -g -c insert.c

delete.o: delete.c
	gcc -g -c delete.c
