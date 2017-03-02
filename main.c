#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 100
#define INSERT "insert"
#define DELETE "delete"
#define FIND "find"
#define CALLER "caller"
#define LOOKUP "lookup"
#define CALLEE "callee"
#define INDIST1 "indist1"
#define TOPDEST "topdest"
#define TOP "top"
#define BYE "bye"
#define PRINT "print"
#define DUMP "dump"

int main(int argc, char* argv[])
{
	int i, HT1numOfEntries, HT2numOfEntries, bucketSize, readFromFile, duration, type, tarrif, fault_condition;
	char fileName[SIZE], fileLine[SIZE], userLine[SIZE], cdr_uniq_id[SIZE], origNum[SIZE], destNum[SIZE], date[SIZE], time[SIZE];
	char *o = "-o", *h1 = "-h1", *h2 = "-h2", *s = "-s", *split;
	FILE *op;

	if(argc == 9)	//input apo arxeio
	{
		readFromFile = 0;
		for(i=1; i<9; i=i+2)
		{
			if(strcmp(argv[i], o) == 0)
			{
				readFromFile = 1;
				strcpy(fileName, argv[i+1]);
			}
			else if(strcmp(argv[i], h1) == 0)
				HT1numOfEntries = atoi(argv[i+1]);
			else if(strcmp(argv[i], h2) == 0)
				HT2numOfEntries = atoi(argv[i+1]);
			else if(strcmp(argv[i], s) == 0)
				bucketSize = atoi(argv[i+1]);
			else
			{
				printf("Wrong flag was given.\n");
				return -1;
			}
		}
		if(readFromFile == 1)
		{
			op = fopen(fileName, "r");
			while(fgets(fileLine, SIZE, op) != NULL)
			{
				split = strtok(fileLine, " ;\r\n");
				//printf("%s\n", fileLine);
				//printf("%s\n", split);

				if(strcmp(split, INSERT) == 0)
				{
					split = strtok(NULL, " ;\r\n");
					strcpy(cdr_uniq_id, split);
					split = strtok(NULL, " ;\r\n");
					strcpy(origNum, split);
					split = strtok(NULL, " ;\r\n");
					strcpy(destNum, split);
					split = strtok(NULL, " ;\r\n");
					strcpy(date, split);
					split = strtok(NULL, " ;\r\n");
					strcpy(time, split);
					split = strtok(NULL, " ;\r\n");
					duration = atoi(split);
					split = strtok(NULL, " ;\r\n");
					type = atoi(split);
					split = strtok(NULL, " ;\r\n");
					tarrif = atoi(split);
					split = strtok(NULL, " ;\r\n");
					fault_condition = atoi(split);

				}
				else if(strcmp(split, DELETE) == 0)
				{

				}
				else if(strcmp(split, FIND) == 0)
				{
					split = strtok(NULL, " ;\r\n");
					if(strcmp(split, CALLER) == 0)
					{

					}
				}
				else if(strcmp(split, LOOKUP) == 0)
				{
					split = strtok(NULL, " ;\r\n");
					if(strcmp(split, CALLEE) == 0)
					{

					}
				}
				else if(strcmp(split, INDIST1) == 0)
				{

				}
				else if(strcmp(split, TOPDEST) == 0)
				{

				}
				else if(strcmp(split, TOP) == 0)
				{

				}
				else if(strcmp(split, BYE) == 0)
				{

				}
				else if(strcmp(split, PRINT) == 0)
				{

				}
				else if(strcmp(split, DUMP) == 0)
				{

				}
			}
		}
		else
		{
			printf("No input file given.\n");
			return -1;
		}
		fclose(op);		
	}




	if(argc == 7)	//input apo stdin
	{
		for(i=1; i<7; i=i+2)
		{
			if(strcmp(argv[i], h1) == 0)
				HT1numOfEntries = atoi(argv[i+1]);
			else if(strcmp(argv[i], h2) == 0)
				HT2numOfEntries = atoi(argv[i+1]);
			else if(strcmp(argv[i], s) == 0)
				bucketSize = atoi(argv[i+1]);
			else
			{
				printf("Wrong flag was given.\n");
				return -1;
			}
		}
		printf("You are in control now.\n");
		while(fgets(userLine, SIZE, stdin) != NULL)
		{
			printf("%s\n",userLine);
		}
	}

	


	printf("h1: %d, h2: %d, bucketSize: %d and filename: %s\n",HT1numOfEntries, HT2numOfEntries, bucketSize, fileName);
	return 0;
}