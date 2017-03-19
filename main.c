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
#include "print.h"
#include "top.h"

int main(int argc, char* argv[])
{
	int i, j, k, HT1numOfEntries, HT2numOfEntries, bucketSize, readFromFile, fileInput, duration, type, tarrif, fault_condition, len, bucket1_maxEntries, bucket2_maxEntries, insertFlag, scenario, line;
	double percentage;
	char fileName[SIZE], billingFileName[SIZE], billingFileLine[SIZE],fileLine[SIZE], userLine[SIZE], ht1[] = "hashtable1", ht2[] = "hashtable2";
	char *o = "-o", *h1 = "-h1", *h2 = "-h2", *s = "-s", *b = "-b", *split, *cdr_uniq_id, *origNum, *destNum, *date, *time, *time1, *time2, *date1, *date2, *caller, *caller1, *caller2;
	hashTable1 *HT1;
	hashTable2 *HT2;
	FILE *op, *billing;
	charge c;
	chargeListNode *ptr, *headNodeCharge, *currentListNode;
	heap *hp;

	//heap
	hp = malloc(sizeof(heap));
	hp -> numOfNodes = 0;
	hp -> head = NULL;
	//

	cdr_uniq_id = NULL; origNum = NULL; destNum = NULL; date = NULL; time = NULL; time1 = NULL; time1 = NULL; date1 = NULL; date2 = NULL; caller = NULL; caller1 = NULL; caller2 = NULL;
	fileInput = 0;
	if(argc == 11)	//input apo arxeio
	{
		fileInput = 1;
		insertFlag = 0;
		readFromFile = 0;
		for(i=1; i<11; i=i+2)
		{
			if(strcmp(argv[i], o) == 0)
			{
				readFromFile = 1;
				strcpy(fileName, argv[i+1]);
			}
			else if(strcmp(argv[i], b) == 0)
			{
				strcpy(billingFileName, argv[i+1]);
				billing = fopen(billingFileName, "r");
				line = 0;
				headNodeCharge = NULL;
				while(fgets(billingFileLine, SIZE, billing))
				{
					if(line == 0)
					{
						line++;
						memset(billingFileLine, 0, SIZE);
						continue;
					}
					line++;

					ptr = malloc(sizeof(chargeListNode));
					ptr -> next = headNodeCharge;

					split = strtok(billingFileLine, " ;\r\n");
					ptr -> c.type = atoi(split);

					split = strtok(NULL, " ;\r\n");
					ptr -> c.tarrif = atoi(split);

					split = strtok(NULL, " ;\r\n");
					ptr -> c.cost = atof(split);

					headNodeCharge = ptr;
					memset(billingFileLine, 0, SIZE);
				}

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
				printf("Wrong flag was given\n");
				return -1;
			}
		}

		op = fopen(fileName, "r");
		if(op && readFromFile == 1)
		{
			while(fgets(fileLine, SIZE, op) != NULL)
			{
				split = strtok(fileLine, " ;\r\n");

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
						printf("Max entries per bucket: b1=%d, b2=%d\n\n", bucket1_maxEntries, bucket2_maxEntries);
					}
					//klisi insert sinartisis
					insertCaller(hp, HT1, HT1numOfEntries, bucket1_maxEntries, bucket2_maxEntries, cdr_uniq_id, origNum, destNum, date, time, duration, type, tarrif, fault_condition, headNodeCharge);
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
									printf("No CDRs found (date1 > date2)\n\n");
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
								printf("No CDRs found (date1 > date2)\n\n");
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
									printf("No CDRs found (date1 > date2)\n\n");
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
								printf("No CDRs found (date1 > date2)\n\n");
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
					split = strtok(NULL, " \r\n");
					percentage = atof(split);
					topKPercent(hp, percentage, HT1, HT1numOfEntries);
				}
				else if(strcmp(split, BYE) == 0)
				{
					printf("[ bye ]\n");
					ragnarok1(hp, HT1, HT1numOfEntries);
					ragnarok2(HT2, HT2numOfEntries);
					printf("Memory cleaned\n\n");
				}
				else if(strcmp(split, PRINT) == 0)
				{
					split = strtok(NULL, " \r\n");
					if(strcmp(split, ht1) == 0)
						printHT1(HT1, HT1numOfEntries);
					else if(strcmp(split, ht2) == 0)
						printHT2(HT2, HT2numOfEntries);
					else
						printf("Wrong argument. Try 'hashtable1' or 'hashtable2'\n");
				}
				else
					printf("Wrong command was given\n");
			}//end_while
			fclose(op);
		}
		else
		{
			printf("Input file does not exist\n");
		}		
	}

	//input apo stdin
	if(fileInput == 0)	//an den exei dothei orisma gia eisodo apo arxeio.
	{
		for(i=1; i<9; i=i+2)
		{
			if(strcmp(argv[i], b) == 0)
			{
				strcpy(billingFileName, argv[i+1]);
				billing = fopen(billingFileName, "r");
				line = 0;
				headNodeCharge = NULL;
				while(fgets(billingFileLine, SIZE, billing))
				{
					if(line == 0)
					{
						line++;
						memset(billingFileLine, 0, SIZE);
						continue;
					}
					line++;

					ptr = malloc(sizeof(chargeListNode));
					ptr -> next = headNodeCharge;

					split = strtok(billingFileLine, " ;\r\n");
					ptr -> c.type = atoi(split);

					split = strtok(NULL, " ;\r\n");
					ptr -> c.tarrif = atoi(split);

					split = strtok(NULL, " ;\r\n");
					ptr -> c.cost = atof(split);

					headNodeCharge = ptr;
					memset(billingFileLine, 0, SIZE);
				}
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
				printf("Wrong flag was given\n");
				return -1;
			}
		}
	}
	/******************************************************************************************/
	/***************************************** USER ******************************************/
	printf("You are in control. Use CTRL-D if you want to exit the program.\n");
	printf("> ");
	while(fgets(userLine, SIZE, stdin) != NULL)
	{
		if(userLine[0] == '\n' || userLine[0] == '\t' || userLine[0] == ' ' || userLine[0] == '\v')
		{
			memset(userLine, 0, SIZE);
			printf("> ");
			continue;
		}

		split = strtok(userLine, " ;\r\n");

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
			insertCaller(hp, HT1, HT1numOfEntries, bucket1_maxEntries, bucket2_maxEntries, cdr_uniq_id, origNum, destNum, date, time, duration, type, tarrif, fault_condition, headNodeCharge);
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
							printf("No CDRs found (date1 > date2)\n\n");
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
						printf("No CDRs found (date1 > date2)\n\n");
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
							printf("No CDRs found (date1 > date2)\n\n");
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
						printf("No CDRs found (date1 > date2)\n");
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
			split = strtok(NULL, " \r\n");
			percentage = atof(split);
			topKPercent(hp, percentage, HT1, HT1numOfEntries);
		}
		else if(strcmp(split, BYE) == 0)
		{
			printf("[ bye ]\n");
			ragnarok1(hp, HT1, HT1numOfEntries);
			ragnarok2(HT2, HT2numOfEntries);
			printf("Memory cleaned\n\n");
		}
		else if(strcmp(split, PRINT) == 0)
		{
			split = strtok(NULL, " \r\n");
			if(strcmp(split, ht1) == 0)
				printHT1(HT1, HT1numOfEntries);
			else if(strcmp(split, ht2) == 0)
				printHT2(HT2, HT2numOfEntries);
			else
				printf("Wrong argument. Try 'hashtable1' or 'hashtable2'\n");
		}
		else
			printf("Wrong command was given\n");

		memset(userLine, 0, SIZE);
		printf("> ");
	}//end while	

	printf("Werhauz terminated\n");

	fclose(billing);

	currentListNode = headNodeCharge;
	for(k=0; k<line-1; k++)
	{
		headNodeCharge = currentListNode -> next;
		free(currentListNode);
		currentListNode = headNodeCharge;
	}
	//klisi sinartiseon katharismou mnimis se periptosi pou de dithei bye oute apo to arxeio oute kai apo ton xristi
	ragnarok1(hp, HT1, HT1numOfEntries);
	ragnarok2(HT2, HT2numOfEntries);
	free(hp);
	free(HT1);
	free(HT2);
	return 0;
}