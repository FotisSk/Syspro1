#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definition.h"
#include "insert.h"

int main(int argc, char* argv[])
{
	int i, j, HT1numOfEntries, HT2numOfEntries, bucketSize, readFromFile, duration, type, tarrif, fault_condition, len, bucket1_maxEntries, bucket2_maxEntries, insertFlag;
	char fileName[SIZE], fileLine[SIZE], userLine[SIZE];
	char *o = "-o", *h1 = "-h1", *h2 = "-h2", *s = "-s", *split, *cdr_uniq_id, *origNum, *destNum, *date, *time;
	hashTable1 *HT1;
	//hashTable2 *HT2;
	FILE *op;

	if(argc == 9)	//input apo arxeio
	{
		insertFlag = 0;
		readFromFile = 0;
		for(i=1; i<9; i=i+2)
		{
			if(strcmp(argv[i], o) == 0)
			{
				readFromFile = 1;
				strcpy(fileName, argv[i+1]);
			}
			else if(strcmp(argv[i], h1) == 0)
			{
				HT1numOfEntries = atoi(argv[i+1]);
				HT1 = malloc(HT1numOfEntries * sizeof(hashTable1));
				for(j=0; j<HT1numOfEntries; j++)	//arxikopoihsh HT1
				{
					HT1[j].numOfNodes1 = 0;
					HT1[j].head1 = NULL;
				}
			}
			else if(strcmp(argv[i], h2) == 0)
			{
				HT2numOfEntries = atoi(argv[i+1]);
			/*	HT2 = malloc(HT2numOfEntries * sizeof(hashTable2));
				for(j=0; j<HT2numOfEntries; j++)	//arxikopoihsh HT2
				{
					HT2[j].numOfNodes2 = 0;
					HT2[j].head2 = NULL;
				}
			*/
			}
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
					len = strlen(split);
					cdr_uniq_id = malloc((len+1)*sizeof(char));
					strcpy(cdr_uniq_id, split);

					split = strtok(NULL, " ;\r\n");
					len = strlen(split);
					origNum = malloc((len+1)*sizeof(char));
					strcpy(origNum, split);

					split = strtok(NULL, " ;\r\n");
					len = strlen(split);
					destNum = malloc((len+1)*sizeof(char));
					strcpy(destNum, split);

					split = strtok(NULL, " ;\r\n");
					len = strlen(split);
					date = malloc((len+1)*sizeof(char));
					strcpy(date, split);

					split = strtok(NULL, " ;\r\n");
					len = strlen(split);
					time = malloc((len+1)*sizeof(char));
					strcpy(time, split);

					split = strtok(NULL, " ;\r\n");
					duration = atoi(split);

					split = strtok(NULL, " ;\r\n");
					type = atoi(split);

					split = strtok(NULL, " ;\r\n");
					tarrif = atoi(split);

					split = strtok(NULL, " ;\r\n");
					fault_condition = atoi(split);

					if(insertFlag == 0)
					{
						bucket1_maxEntries = bucketSize / (sizeof(origNum) + sizeof(bucketNode2*));
						bucket2_maxEntries = bucketSize / (sizeof(cdr_uniq_id) + sizeof(destNum) + sizeof(date) + sizeof(time) + sizeof(duration) + sizeof(type) + sizeof(tarrif) + sizeof(fault_condition));
					}
					//klisi insert sinartisis
					insert(HT1, HT1numOfEntries, bucket1_maxEntries, bucket2_maxEntries, cdr_uniq_id, origNum, destNum, date, time, duration, type, tarrif, fault_condition);
					insertFlag = 1;
					//free gia na min exoume leaks
					free(cdr_uniq_id); free(origNum); free(destNum); free(date); free(time);
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
			}//end_while
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