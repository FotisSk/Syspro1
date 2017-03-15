#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definition.h"
#include "insert.h"
#include "delete.h"
#include "find.h"
#include "lookup.h"
#include "indist1.h"
#include "topdest.h"
#include "bye.h"


int main(int argc, char* argv[])
{
	int i, j, HT1numOfEntries, HT2numOfEntries, bucketSize, readFromFile, duration, type, tarrif, fault_condition, len, bucket1_maxEntries, bucket2_maxEntries, insertFlag, scenario;
	char fileName[SIZE], fileLine[SIZE], userLine[SIZE];
	char *o = "-o", *h1 = "-h1", *h2 = "-h2", *s = "-s", *split, *cdr_uniq_id, *origNum, *destNum, *date, *time, *time1, *time2, *date1, *date2, *caller, *caller1, *caller2;
	hashTable1 *HT1;
	hashTable2 *HT2;
	FILE *op;

	cdr_uniq_id = NULL; origNum = NULL; destNum = NULL; date = NULL; time = NULL; time1 = NULL; time1 = NULL; date1 = NULL; date2 = NULL; caller = NULL; caller1 = NULL; caller2 = NULL;
	
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
				HT2 = malloc(HT2numOfEntries * sizeof(hashTable2));
				for(j=0; j<HT2numOfEntries; j++)	//arxikopoihsh HT2
				{
					HT2[j].numOfNodes2 = 0;
					HT2[j].head2 = NULL;
				}
			
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
						bucket1_maxEntries = bucketSize / sizeof(bucket1_caller);
						bucket2_maxEntries = bucketSize / sizeof(bucket2_caller);
						printf("MAX ENTRIES: b1=%d, b2=%d\n", bucket1_maxEntries, bucket2_maxEntries);
					}
					//klisi insert sinartisis
					insertCaller(HT1, HT1numOfEntries, bucket1_maxEntries, bucket2_maxEntries, cdr_uniq_id, origNum, destNum, date, time, duration, type, tarrif, fault_condition);
					insertCallee(HT2, HT2numOfEntries, bucket1_maxEntries, bucket2_maxEntries, cdr_uniq_id, origNum, destNum, date, time, duration, type, tarrif, fault_condition);
					
					insertFlag = 1;
					//free gia na min exoume leaks
					free(cdr_uniq_id);
					cdr_uniq_id = NULL;
					free(origNum);
					origNum = NULL;
					free(destNum);
					destNum = NULL;
					free(date);
					date = NULL;
					free(time);
					time = NULL;
				}
				else if(strcmp(split, DELETE) == 0)
				{
					split = strtok(NULL, " ;\r\n");
					len = strlen(split);
					cdr_uniq_id = malloc((len+1)*sizeof(char));
					strcpy(cdr_uniq_id, split);

					split = strtok(NULL, " ;\r\n");
					len = strlen(split);
					origNum = malloc((len+1)*sizeof(char));
					strcpy(origNum, split);

					delete(HT1, HT1numOfEntries, cdr_uniq_id, origNum);
					//free gia na min exoume leaks
					free(origNum);
					origNum = NULL;
					free(cdr_uniq_id);
					cdr_uniq_id = NULL;
				}
				else if(strcmp(split, FIND) == 0)
				{
					split = strtok(NULL, " \r\n");
					len = strlen(split);
					origNum = malloc((len+1)*sizeof(char));
					strcpy(origNum, split);

					/************* SCENARIOS *************/
					/* 0: CALLER                         */
					/* 1: CALLER TIME1 TIME2             */
					/* 2: CALLER DATE1 DATE2             */
					/* 3: CALLER TIME1 DATE1 TIME2 DATE2 */
					/*************************************/

					scenario = -1;

					split = strtok(NULL, " \r\n");
					if(split)	//an einai diaforo tou NULL tote simainei oti mas edosan sigoura date h/kai time
					{
						len = strlen(split);
						if(len == 5)	//to proto orisma einai time1
						{
							time1 = malloc((len+1)*sizeof(char));
							strcpy(time1, split);

							split = strtok(NULL, " \r\n");
							len = strlen(split);
							if(len == 5)	//to deutero orisma einai time2
							{
								time2 = malloc((len+1)*sizeof(char));
								strcpy(time2, split);

								scenario = 1;
								findCaller(HT1, HT1numOfEntries, scenario, origNum, time1, date1, time2, date2);
								free(origNum);
								origNum = NULL;
								free(time1);
								time1 = NULL;
								free(time2);
								time2 = NULL;
							}
							else if(len == 8)	//to deutero orisma einai date1, ara to trito tha einai time2 kai to tetarto date2
							{
								date1 = malloc((len+1)*sizeof(char));
								strcpy(date1, split);

								split = strtok(NULL, " \r\n");
								len = strlen(split);
								time2 = malloc((len+1)*sizeof(char));
								strcpy(time2, split);

								split = strtok(NULL, " \r\n");
								len = strlen(split);
								date2 = malloc((len+1)*sizeof(char));
								strcpy(date2, split);

								scenario = 3;
								
								if(findCaller(HT1, HT1numOfEntries, scenario, origNum, time1, date1, time2, date2) == -1)
								{
									printf("> Wrong dates were given (date1 > date2)\n");
									printf("|--------------------------------------------------------------------------------------------------------|\n");
								}
								free(origNum);
								origNum = NULL;
								free(time1);
								time1 = NULL;
								free(date1);
								date1 = NULL;
								free(time2);
								time2 = NULL;
								free(date2);
								date2 = NULL;
							}
						}
						else if(len == 8)	//to proto orisma einai date1, tote sigoura to deutero orisma einai date2
						{
							date1 = malloc((len+1)*sizeof(char));
							strcpy(date1, split);

							split = strtok(NULL, " \r\n");
							len = strlen(split);
							date2 = malloc((len+1)*sizeof(char));
							strcpy(date2, split);

							scenario = 2;
							
							if(findCaller(HT1, HT1numOfEntries, scenario, origNum, time1, date1, time2, date2) == -1)
							{
								printf("> Wrong dates were given (date1 > date2)\n");
								printf("|--------------------------------------------------------------------------------------------------------|\n");
							}
							free(origNum);
							origNum = NULL;
							free(date1);
							date1 = NULL;
							free(date2);
							date2 = NULL;
						}
					}
					else
					{
						scenario = 0;
						findCaller(HT1, HT1numOfEntries, scenario, origNum, time1, date1, time2, date2);
						free(origNum);
						origNum = NULL;		
					}
				}
				else if(strcmp(split, LOOKUP) == 0)
				{
					split = strtok(NULL, " \r\n");
					len = strlen(split);
					destNum = malloc((len+1)*sizeof(char));
					strcpy(destNum, split);

					/************* SCENARIOS *************/
					/* 0: CALLEE                         */
					/* 1: CALLEE TIME1 TIME2             */
					/* 2: CALLEE DATE1 DATE2             */
					/* 3: CALLEE TIME1 DATE1 TIME2 DATE2 */
					/*************************************/

					scenario = -1;

					split = strtok(NULL, " \r\n");
					if(split)	//an einai diaforo tou NULL tote simainei oti mas edosan sigoura date h/kai time
					{
						len = strlen(split);
						if(len == 5)	//to proto orisma einai time1
						{
							time1 = malloc((len+1)*sizeof(char));
							strcpy(time1, split);

							split = strtok(NULL, " \r\n");
							len = strlen(split);
							if(len == 5)	//to deutero orisma einai time2
							{
								time2 = malloc((len+1)*sizeof(char));
								strcpy(time2, split);

								scenario = 1;
								lookupCallee(HT2, HT2numOfEntries, scenario, destNum, time1, date1, time2, date2);
								free(destNum);
								destNum = NULL;
								free(time1);
								time1 = NULL;
								free(time2);
								time2 = NULL;
							}
							else if(len == 8)	//to deutero orisma einai date1, ara to trito tha einai time2 kai to tetarto date2
							{
								date1 = malloc((len+1)*sizeof(char));
								strcpy(date1, split);

								split = strtok(NULL, " \r\n");
								len = strlen(split);
								time2 = malloc((len+1)*sizeof(char));
								strcpy(time2, split);

								split = strtok(NULL, " \r\n");
								len = strlen(split);
								date2 = malloc((len+1)*sizeof(char));
								strcpy(date2, split);

								scenario = 3;
								if(lookupCallee(HT2, HT2numOfEntries, scenario, destNum, time1, date1, time2, date2) == -1)
								{
									printf("> Wrong dates were given (date1 > date2)\n");
									printf("|--------------------------------------------------------------------------------------------------------|\n");
								}
								free(destNum);
								destNum = NULL;
								free(time1);
								time1 = NULL;
								free(date1);
								date1 = NULL;
								free(time2);
								time2 = NULL;
								free(date2);
								date2 = NULL;
							}
						}
						else if(len == 8)	//to proto orisma einai date1, tote sigoura to deutero orisma einai date2
						{
							date1 = malloc((len+1)*sizeof(char));
							strcpy(date1, split);

							split = strtok(NULL, " \r\n");
							len = strlen(split);
							date2 = malloc((len+1)*sizeof(char));
							strcpy(date2, split);

							scenario = 2;
							if(lookupCallee(HT2, HT2numOfEntries, scenario, destNum, time1, date1, time2, date2) == -1)
							{
								printf("> Wrong dates were given (date1 > date2)\n");
								printf("|--------------------------------------------------------------------------------------------------------|\n");
							}
							free(destNum);
							destNum = NULL;	
							free(date1);
							date1 = NULL;
							free(date2);
							date2 = NULL;
						}
					}
					else
					{
						scenario = 0;
						lookupCallee(HT2, HT2numOfEntries, scenario, destNum, time1, date1, time2, date2);
						free(destNum);
						destNum = NULL;		
					}
				}
				else if(strcmp(split, INDIST1) == 0)
				{
					split = strtok(NULL, " \r\n");
					len = strlen(split);
					caller1 = malloc((len+1)*sizeof(char));
					strcpy(caller1, split);

					split = strtok(NULL, " \r\n");
					len = strlen(split);
					caller2 = malloc((len+1)*sizeof(char));
					strcpy(caller2, split);

					indist(HT1, HT1numOfEntries, HT2, HT2numOfEntries, caller1, caller2);
					free(caller1);
					caller1 = NULL;
					free(caller2);
					caller2 = NULL;

				}
				else if(strcmp(split, TOPDEST) == 0)
				{
					split = strtok(NULL, " \r\n");
					len = strlen(split);
					caller = malloc((len+1)*sizeof(char));
					strcpy(caller, split);

					topDestCaller(HT1, HT1numOfEntries, caller);
					free(caller);
					caller = NULL;
				}
				else if(strcmp(split, TOP) == 0)
				{

				}
				else if(strcmp(split, BYE) == 0)
				{
					ragnarok1(HT1, HT1numOfEntries);
					ragnarok2(HT2, HT2numOfEntries);
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
	free(HT1);
	free(HT2);
	return 0;
}