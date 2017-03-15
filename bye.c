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

void ragnarok1(hashTable1 *HT1, int HT1numOfEntries)
{
	int i, j, k;
	bucketNode1_caller *currentBucketNode1;
	bucketNode2_caller *currentBucketNode2;
	
	for(i=0; i<HT1numOfEntries; i++)
	{
		//previousBucketNode1 = NULL;
		currentBucketNode1 = HT1[i].head1;
		while(currentBucketNode1)
		{
			for(j=0; j<currentBucketNode1 -> nextAvailablePos; j++)
			{
				//previousBucketNode2 = NULL;
				currentBucketNode2 = currentBucketNode1 -> b1[j].extraCDR;

				while(currentBucketNode2)
				{
					for(k=0; k<currentBucketNode2 -> nextAvailablePos; k++)
					{
						free(currentBucketNode2 -> b2[k].cdr_uniq_id);
						currentBucketNode2 -> b2[k].cdr_uniq_id = NULL;
						free(currentBucketNode2 -> b2[k].destNum);
						currentBucketNode2 -> b2[k].destNum = NULL;
					}
					free(currentBucketNode2 -> b2);
					currentBucketNode2 -> b2 = NULL;

					currentBucketNode1 -> b1[j].extraCDR = currentBucketNode2 -> next;
					free(currentBucketNode2);
					//previousBucketNode2 = currentBucketNode2;
					currentBucketNode2 = currentBucketNode1 -> b1[j].extraCDR;
				}
				free(currentBucketNode1 -> b1[j].origNum);
				currentBucketNode1 -> b1[j].origNum = NULL;
			}
			free(currentBucketNode1 -> b1);
			currentBucketNode1 -> b1 = NULL;

			HT1[i].head1 = currentBucketNode1 -> next;
			free(currentBucketNode1);
			currentBucketNode1 = HT1[i].head1;
		}
		HT1[i].numOfNodes1 = 0;
	}
	//free(HT1);
}

void ragnarok2(hashTable2 *HT2, int HT2numOfEntries)
{
	int i, j, k;
	bucketNode1_callee *currentBucketNode1;
	bucketNode2_callee *currentBucketNode2;
	
	for(i=0; i<HT2numOfEntries; i++)
	{
		//previousBucketNode1 = NULL;
		currentBucketNode1 = HT2[i].head2;
		while(currentBucketNode1)
		{
			for(j=0; j<currentBucketNode1 -> nextAvailablePos; j++)
			{
				//previousBucketNode2 = NULL;
				currentBucketNode2 = currentBucketNode1 -> b1[j].extraCDR;

				while(currentBucketNode2)
				{
					for(k=0; k<currentBucketNode2 -> nextAvailablePos; k++)
					{
						free(currentBucketNode2 -> b2[k].cdr_uniq_id);
						currentBucketNode2 -> b2[k].cdr_uniq_id = NULL;
						free(currentBucketNode2 -> b2[k].origNum);
						currentBucketNode2 -> b2[k].origNum = NULL;
					}
					free(currentBucketNode2 -> b2);
					currentBucketNode2 -> b2 = NULL;

					currentBucketNode1 -> b1[j].extraCDR = currentBucketNode2 -> next;
					free(currentBucketNode2);
					//previousBucketNode2 = currentBucketNode2;
					currentBucketNode2 = currentBucketNode1 -> b1[j].extraCDR;
				}
				free(currentBucketNode1 -> b1[j].destNum);
				currentBucketNode1 -> b1[j].destNum = NULL;
			}
			free(currentBucketNode1 -> b1);
			currentBucketNode1 -> b1 = NULL;

			HT2[i].head2 = currentBucketNode1 -> next;
			free(currentBucketNode1);
			currentBucketNode1 = HT2[i].head2;
		}
		HT2[i].numOfNodes2 = 0;
	}
	//free(HT2);
}