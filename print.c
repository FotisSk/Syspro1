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

void printHT1(hashTable1 *HT1, int HT1numOfEntries)
{
	int i, j, k;
	bucketNode1_caller *currentBucketNode1;
	bucketNode2_caller *currentBucketNode2;
	bucket2_caller *b2;

	printf("[ print hashtable1 ]\n");
	printf("HASHTABLE_1\n");
	for(i=0; i<HT1numOfEntries; i++)
	{
		printf("           HT1[%d]\n", i);
		currentBucketNode1 = HT1[i].head1;
		while(currentBucketNode1)
		{
			for(j=0; j<currentBucketNode1 -> nextAvailablePos; j++)
			{
				printf("                [origNum: %s]\n", currentBucketNode1 -> b1[j].origNum);
				currentBucketNode2 = currentBucketNode1 -> b1[j].extraCDR;
				while(currentBucketNode2)
				{
					for(k=0; k<currentBucketNode2 -> nextAvailablePos; k++)
					{
						b2 = currentBucketNode2 -> b2;
						printf("                   * cdr_uniq_id: %s, destNum: %s, date: %d/%d/%d, time: %dH:%dM, duration: %d, type: %d, tarrif: %d, fault_condition: %d\n", b2[k].cdr_uniq_id, b2[k].destNum, b2[k].date.day
																																							,b2[k].date.month, b2[k].date.year, b2[k].time.hours
																																								,b2[k].time.minutes, b2[k].duration, b2[k].type, b2[k].tarrif
																																									,b2[k].fault_condition);
					}

					currentBucketNode2 = currentBucketNode2 -> next;
				}
			}
			currentBucketNode1 = currentBucketNode1 -> next;
		}
	}
	printf("\n");
}


void printHT2(hashTable2 *HT2, int HT2numOfEntries)
{
	int i, j, k;
	bucketNode1_callee *currentBucketNode1;
	bucketNode2_callee *currentBucketNode2;
	bucket2_callee *b2;

	printf("[ print hashtable2 ]\n");
	printf("HASHTABLE_2\n");
	for(i=0; i<HT2numOfEntries; i++)
	{
		printf("           HT2[%d]\n", i);
		currentBucketNode1 = HT2[i].head2;
		while(currentBucketNode1)
		{
			for(j=0; j<currentBucketNode1 -> nextAvailablePos; j++)
			{
				printf("                [destNum: %s]\n", currentBucketNode1 -> b1[j].destNum);
				currentBucketNode2 = currentBucketNode1 -> b1[j].extraCDR;
				while(currentBucketNode2)
				{
					for(k=0; k<currentBucketNode2 -> nextAvailablePos; k++)
					{
						b2 = currentBucketNode2 -> b2;
						printf("                   * cdr_uniq_id: %s, origNum: %s, date: %d/%d/%d, time: %dH:%dM, duration: %d, type: %d, tarrif: %d, fault_condition: %d\n", b2[k].cdr_uniq_id, b2[k].origNum, b2[k].date.day
																																							,b2[k].date.month, b2[k].date.year, b2[k].time.hours
																																								,b2[k].time.minutes, b2[k].duration, b2[k].type, b2[k].tarrif
																																									,b2[k].fault_condition);
					}

					currentBucketNode2 = currentBucketNode2 -> next;
				}
			}
			currentBucketNode1 = currentBucketNode1 -> next;
		}
	}
	printf("\n");
}