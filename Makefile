all: main.o insert.o delete.o find.o lookup.o indist1.o topdest.o top.o bye.o print.o
	gcc -o werhauz main.o insert.o delete.o find.o lookup.o indist1.o topdest.o top.o bye.o print.o -g

main.o: main.c
	gcc -g -c main.c

insert.o: insert.c
	gcc -g -c insert.c

delete.o: delete.c
	gcc -g -c delete.c

find.o: find.c
	gcc -g -c find.c

lookup.o: lookup.c
	gcc -g -c lookup.c

indist1.o: indist1.c
	gcc -c -g indist1.c

topdest.o: topdest.c
	gcc -c -g topdest.c

top.o: top.c
	gcc -c -g top.c

bye.o: bye.c
	gcc -c -g bye.c

print.o: print.c
	gcc -c -g print.c

