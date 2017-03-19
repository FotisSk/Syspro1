#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definition.h"
#include "insert.h"
#include "delete.h"

int delete(hashTable1 *HT1, int HT1numOfEntries, char *cdr_uniq_id, char *origNum)
{
	int key, numOfNodes1, flag1, flag2, i, j, k, currentPos, lastPos;
	bucketNode1_caller *head1, *currentBucketNode1;
	bucketNode2_caller *currentBucketNode2, *previousBucketNode2;

	printf("[ delete %s %s ]\n", origNum, cdr_uniq_id);
	key = hashFunction(origNum, HT1numOfEntries);

	numOfNodes1 = HT1[key].numOfNodes1;
	head1 = HT1[key].head1;
	currentBucketNode1 = head1;
	flag1 = 0;
	i = 0;
	
	while(i<numOfNodes1 && flag1 == 0)
	{
		j = 0;
		while(j<currentBucketNode1 -> nextAvailablePos && flag1 == 0)
		{
			if(strcmp(currentBucketNode1 -> b1[j].origNum, origNum) == 0)	//vrika ton arithmo pou ziteitai, tora psaxno tin eggrafi me to sigekrimeno cdr_uniq_id.
			{
				if(currentBucketNode1 -> b1[j].numOfNodes2 == 0)
				{
					printf("DError (No calls found from caller '%s')\n\n", origNum);
					return -1;
				}
				flag1 = 1;
				currentBucketNode2 = currentBucketNode1 -> b1[j].extraCDR;
				previousBucketNode2 = NULL;
				flag2 = 0;
				while(currentBucketNode2 && flag2 == 0)
				{
					k = 0;
					while(k<currentBucketNode2 -> nextAvailablePos && flag2 == 0)
					{
						if(strcmp(currentBucketNode2 -> b2[k].cdr_uniq_id, cdr_uniq_id) == 0)	//tote svistin.
						{
							flag2 = 1;
							free(currentBucketNode2 -> b2[k].cdr_uniq_id);
							currentBucketNode2 -> b2[k].cdr_uniq_id = NULL;
							free(currentBucketNode2 -> b2[k].destNum);
							currentBucketNode2 -> b2[k].destNum = NULL;
							
							currentBucketNode2 -> b2[k].date.day = 0;
							currentBucketNode2 -> b2[k].date.month = 0;
							currentBucketNode2 -> b2[k].date.year = 0;
							
							currentBucketNode2 -> b2[k].time.hours = 0;
							currentBucketNode2 -> b2[k].time.minutes = 0;
							currentBucketNode2 -> b2[k].time.totalMinutes = 0;

							currentBucketNode2 -> b2[k].duration = 0;
							currentBucketNode2 -> b2[k].type = 0;
							currentBucketNode2 -> b2[k].tarrif = 0;
							currentBucketNode2 -> b2[k].fault_condition = 0;

							printf("Deleted %s\n", cdr_uniq_id);

							if(k < currentBucketNode2 -> nextAvailablePos - 1)	//tote den esvises tin teleutaia sto bucket2 opote thelei mia anadiataksi.
							{
								//san lisi tha fero tin teleutaia eggrafi tou bucket2 sti thesi tis svismenis eggrafis.
								currentPos = k;
								lastPos = currentBucketNode2 -> nextAvailablePos - 1;

								currentBucketNode2 -> b2[currentPos].cdr_uniq_id = malloc((strlen(currentBucketNode2 -> b2[lastPos].cdr_uniq_id)+1) * sizeof(char));
								strcpy(currentBucketNode2 -> b2[currentPos].cdr_uniq_id, currentBucketNode2 -> b2[lastPos].cdr_uniq_id );

								currentBucketNode2 -> b2[currentPos].destNum = malloc((strlen(currentBucketNode2 -> b2[lastPos].destNum)+1) * sizeof(char));
								strcpy(currentBucketNode2 -> b2[currentPos].destNum, currentBucketNode2 -> b2[lastPos].destNum );

								currentBucketNode2 -> b2[currentPos].date = currentBucketNode2 -> b2[lastPos].date;
								currentBucketNode2 -> b2[currentPos].time = currentBucketNode2 -> b2[lastPos].time;

								currentBucketNode2 -> b2[currentPos].duration = currentBucketNode2 -> b2[lastPos].duration;
								currentBucketNode2 -> b2[currentPos].type = currentBucketNode2 -> b2[lastPos].type;
								currentBucketNode2 -> b2[currentPos].tarrif = currentBucketNode2 -> b2[lastPos].tarrif;
								currentBucketNode2 -> b2[currentPos].fault_condition = currentBucketNode2 -> b2[lastPos].fault_condition;

								//tora kano free tin lastPos
								free(currentBucketNode2 -> b2[lastPos].cdr_uniq_id);
								currentBucketNode2 -> b2[lastPos].cdr_uniq_id = NULL;
								free(currentBucketNode2 -> b2[lastPos].destNum);
								currentBucketNode2 -> b2[lastPos].destNum = NULL;
								
								currentBucketNode2 -> b2[lastPos].date.day = 0;
								currentBucketNode2 -> b2[lastPos].date.month = 0;
								currentBucketNode2 -> b2[lastPos].date.year= 0;
								
								currentBucketNode2 -> b2[lastPos].time.hours = 0;
								currentBucketNode2 -> b2[lastPos].time.minutes = 0;
								currentBucketNode2 -> b2[lastPos].time.totalMinutes = 0;

								currentBucketNode2 -> b2[lastPos].duration = 0;
								currentBucketNode2 -> b2[lastPos].type = 0;
								currentBucketNode2 -> b2[lastPos].tarrif = 0;
								currentBucketNode2 -> b2[lastPos].fault_condition = 0;
							}

							currentBucketNode2 -> nextAvailablePos--;

							if(currentBucketNode2 -> nextAvailablePos == 0)	//tote diagrapsame tin monadiki eggrafi tou bucket2
							{
								if(previousBucketNode2)	//an den einai NULL diladi o current vrisketai pera apo to 1o bucket
								{
									previousBucketNode2 -> next = currentBucketNode2 -> next;

									free(currentBucketNode2 -> b2);
									currentBucketNode2 -> b2 = NULL;
									free(currentBucketNode2);
									currentBucketNode2 = NULL;

									currentBucketNode1 -> b1[j].numOfNodes2--;
								}
								else	//prepei na diagrapsoume to 1o bucket, eite eiani to monadiko eite oxi
								{
									currentBucketNode1 -> b1[j].extraCDR = currentBucketNode2 -> next;

									free(currentBucketNode2 -> b2);
									currentBucketNode2 -> b2 = NULL;
									free(currentBucketNode2);
									currentBucketNode2 = NULL;

									currentBucketNode1 -> b1[j].numOfNodes2--;
								}
								printf("\n");
								return 1;
							}
						}
						else
							k++;
					}
					previousBucketNode2 = currentBucketNode2;
					currentBucketNode2 = currentBucketNode2 -> next;
				}
				if(flag2 == 0)
				{
					printf("DError  (Caller '%s' has no call with cdr_uniq_id '%s')\n\n", origNum, cdr_uniq_id);
					return -1;
				}
			}
			else
				j++;
		}
		if(i != numOfNodes1-1)	//oste stin teleutaia epanalipsi na min deiksei NULL
				currentBucketNode1 = currentBucketNode1 -> next;
		i++;
	}
	if(flag1 == 0)
	{
		printf("DError (Caller '%s' not found)\n\n", origNum);
		return -1;
	}
	printf("\n");
}