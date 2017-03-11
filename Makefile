all: main.o insert.o delete.o find_caller.o
	gcc -o werhauz main.o insert.o delete.o find_caller.o -g

main.o: main.c
	gcc -g -c main.c

insert.o: insert.c
	gcc -g -c insert.c

delete.o: delete.c
	gcc -g -c delete.c

find_caller.o: find_caller.c
	gcc -g -c find_caller.c

