#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definition.h"
#include "insert.h"

int hashFunction(char *origNum, int HT1numOfEntries)
{
	int len, sum, i;
	char digit[2];

	sum = 33;
	len = strlen(origNum);
	for(i=0; i<len; i++)
	{
		if(i != 3)
		{
			digit[0] = origNum[i];
			sum = sum + atoi(digit);
		}
		else 
			continue;
	}
	return (sum % HT1numOfEntries);
}

int insert(hashTable1 *HT1, int HT1numOfEntries, int bucket1_maxEntries, int bucket2_maxEntries,
 char *cdr_uniq_id, char *origNum, char *destNum, char *date, char *time,
  int duration, int type, int tarrif, int fault_condition)
{
	int key, nextAvailablePos1, nextAvailablePos2, numOfNodes1, i, j, flag1, flag2;
	bucketNode1 *bn1, *head1, *currentBucketNode1;
	bucketNode2 *currentBucketNode2;

	key = hashFunction(origNum, HT1numOfEntries);

	if(HT1[key].numOfNodes1 == 0)	//den exei ksanabei entry
	{
		bn1 = malloc(sizeof(bucketNode1));
		HT1[key].head1 = bn1;
		HT1[key].numOfNodes1++;

		bn1 -> nextAvailablePos = 1;	//1 kai oxi 0 giati vazo entry sto bucket parallila
		bn1 -> next  = NULL;
		bn1 -> b1 = malloc(bucket1_maxEntries * sizeof(bucket1));
		bn1 -> b1[0].origNum = malloc((strlen(origNum)+1) * sizeof(char));
		strcpy(bn1 -> b1[0].origNum, origNum);

		bn1 -> b1[0].extraCDR = malloc(sizeof(bucketNode2));
		bn1 -> b1[0].extraCDR -> nextAvailablePos = 1;	//1 kai oxi 0 giati vazo entry sto bucket parallila
		bn1 -> b1[0].extraCDR -> next = NULL;
		bn1 -> b1[0].extraCDR -> b2 = malloc(bucket2_maxEntries * sizeof(bucket2));

		bn1 -> b1[0].extraCDR -> b2[0].cdr_uniq_id = malloc((strlen(cdr_uniq_id)+1) * sizeof(char));
		strcpy(bn1 -> b1[0].extraCDR -> b2[0].cdr_uniq_id, cdr_uniq_id);

		bn1 -> b1[0].extraCDR -> b2[0].destNum = malloc((strlen(destNum)+1) * sizeof(char));
		strcpy(bn1 -> b1[0].extraCDR -> b2[0].destNum, destNum);

		bn1 -> b1[0].extraCDR -> b2[0].date = malloc((strlen(date)+1) * sizeof(char));
		strcpy(bn1 -> b1[0].extraCDR -> b2[0].date, date);

		bn1 -> b1[0].extraCDR -> b2[0].time = malloc((strlen(time)+1) * sizeof(char));
		strcpy(bn1 -> b1[0].extraCDR -> b2[0].time, time);

		bn1 -> b1[0].extraCDR -> b2[0].duration = duration;
		bn1 -> b1[0].extraCDR -> b2[0].type = type;
		bn1 -> b1[0].extraCDR -> b2[0].tarrif = tarrif;
		bn1 -> b1[0].extraCDR -> b2[0].fault_condition = fault_condition;
		return 1;
	}
	else	//to HT1[key].head den einai NULL, iparxoun idi entries.
	{
		numOfNodes1 = HT1[key].numOfNodes1;
		head1 = HT1[key].head1;
		currentBucketNode1 = head1;
		flag1 = 0;
		//nextAvailablePos1 = head1 -> nextAvailablePos;
		for(i=0; i<numOfNodes1; i++)	//anazitisi an iparxei idi to origNum.
		{
			for(j=0; j<currentBucketNode1 -> nextAvailablePos; j++)
			{
				if(strcmp(currentBucketNode1 -> b1[j].origNum, origNum) == 0)	//an vreis to tilefono tote pigaine na deis ama xorane ta kainourgia extraCDR.
				{
					flag1 = 1;
					currentBucketNode2 = currentBucketNode1 -> b1[j].extraCDR;
					flag2 = 0;
					while((currentBucketNode2 -> nextAvailablePos == bucket2_maxEntries) && (flag2 == 0))
					{
						if(currentBucketNode2 -> next) //an einai diaforos tou NULL
							currentBucketNode2 = currentBucketNode2 -> next;
						else
							flag2 = 1;
					}

					if(flag2 == 0)	//tote vrikame keni thesi ekei pou deixnei tora o currentBucketNode2
					{
						nextAvailablePos2 = currentBucketNode2 -> nextAvailablePos;
						/***/
						currentBucketNode2 -> b2[nextAvailablePos2].cdr_uniq_id = malloc((strlen(cdr_uniq_id)+1) * sizeof(char));
						strcpy(currentBucketNode2 -> b2[nextAvailablePos2].cdr_uniq_id, cdr_uniq_id);

						currentBucketNode2 -> b2[nextAvailablePos2].destNum = malloc((strlen(destNum)+1) * sizeof(char));
						strcpy(currentBucketNode2 -> b2[nextAvailablePos2].destNum, destNum);

						currentBucketNode2 -> b2[nextAvailablePos2].date = malloc((strlen(date)+1) * sizeof(char));
						strcpy(currentBucketNode2 -> b2[nextAvailablePos2].date, date);

						currentBucketNode2 -> b2[nextAvailablePos2].time = malloc((strlen(time)+1) * sizeof(char));
						strcpy(currentBucketNode2 -> b2[nextAvailablePos2].time, time);

						currentBucketNode2 -> b2[nextAvailablePos2].duration = duration;

						currentBucketNode2 -> b2[nextAvailablePos2].type = type;

						currentBucketNode2 -> b2[nextAvailablePos2].tarrif = tarrif;

						currentBucketNode2 -> b2[nextAvailablePos2].fault_condition = fault_condition;
						/***/
						currentBucketNode2 -> nextAvailablePos++;
						return 1;
					}
					else if(flag2 == 1)	//eftase sto telos kai de vrike keni thesi.
					{
						currentBucketNode2 -> next = malloc(sizeof(bucketNode2));
						currentBucketNode2 = currentBucketNode2 -> next;

						currentBucketNode2 -> nextAvailablePos = 1;	//1 kai oxi 0 giati tha valo parallila mesa tin eggrafi
						currentBucketNode2 -> next = NULL;
						currentBucketNode2 -> b2 = malloc(bucket2_maxEntries * sizeof(bucket2));

						/***/
						currentBucketNode2 -> b2[0].cdr_uniq_id = malloc((strlen(cdr_uniq_id)+1) * sizeof(char));
						strcpy(currentBucketNode2 -> b2[0].cdr_uniq_id, cdr_uniq_id);

						currentBucketNode2 -> b2[0].destNum = malloc((strlen(destNum)+1) * sizeof(char));
						strcpy(currentBucketNode2 -> b2[0].destNum, destNum);

						currentBucketNode2 -> b2[0].date = malloc((strlen(date)+1) * sizeof(char));
						strcpy(currentBucketNode2 -> b2[0].date, date);

						currentBucketNode2 -> b2[0].time = malloc((strlen(time)+1) * sizeof(char));
						strcpy(currentBucketNode2 -> b2[0].time, time);

						currentBucketNode2 -> b2[0].duration = duration;

						currentBucketNode2 -> b2[0].type = type;

						currentBucketNode2 -> b2[0].tarrif = tarrif;

						currentBucketNode2 -> b2[0].fault_condition = fault_condition;
						/***/
						return 1;
					}

				}
			}
			if(i != numOfNodes1-1)	//oste stin teleutaia epanalipsi na min deiksei NULL
				currentBucketNode1 = currentBucketNode1 -> next;
		}

		if(flag1 == 0)	//de vrike idio origNum
		{
			nextAvailablePos1 = currentBucketNode1 -> nextAvailablePos;
			if(nextAvailablePos1 < bucket1_maxEntries)	//an xoraei to entry sto teleutaio bucket1
			{
				currentBucketNode1 -> b1[nextAvailablePos1].origNum = malloc((strlen(origNum)+1) * sizeof(char));
				strcpy(currentBucketNode1 -> b1[nextAvailablePos1].origNum, origNum);
				currentBucketNode1 -> b1[nextAvailablePos1].extraCDR = malloc(sizeof(bucketNode2));
				currentBucketNode1 -> nextAvailablePos++;

				currentBucketNode2 = currentBucketNode1 -> b1[nextAvailablePos1].extraCDR;
				currentBucketNode2 -> nextAvailablePos = 1;
				currentBucketNode2 -> next = NULL;
				currentBucketNode2 -> b2 = malloc(bucket2_maxEntries * sizeof(bucket2));

				/***/
				currentBucketNode2 -> b2[0].cdr_uniq_id = malloc((strlen(cdr_uniq_id)+1) * sizeof(char));
				strcpy(currentBucketNode2 -> b2[0].cdr_uniq_id, cdr_uniq_id);

				currentBucketNode2 -> b2[0].destNum = malloc((strlen(destNum)+1) * sizeof(char));
				strcpy(currentBucketNode2 -> b2[0].destNum, destNum);

				currentBucketNode2 -> b2[0].date = malloc((strlen(date)+1) * sizeof(char));
				strcpy(currentBucketNode2 -> b2[0].date, date);

				currentBucketNode2 -> b2[0].time = malloc((strlen(time)+1) * sizeof(char));
				strcpy(currentBucketNode2 -> b2[0].time, time);

				currentBucketNode2 -> b2[0].duration = duration;

				currentBucketNode2 -> b2[0].type = type;

				currentBucketNode2 -> b2[0].tarrif = tarrif;

				currentBucketNode2 -> b2[0].fault_condition = fault_condition;
				/***/
			}
			else	//an de xoraei to entry sto teleutaio bucket1
			{
				currentBucketNode1 -> next = malloc(sizeof(bucketNode1));
				HT1[key].numOfNodes1++;
				currentBucketNode1 = currentBucketNode1 -> next;
				
				currentBucketNode1 -> nextAvailablePos = 1;
				currentBucketNode1 -> next = NULL;
				currentBucketNode1 -> b1 = malloc(bucket1_maxEntries * sizeof(bucket1));

				currentBucketNode1 -> b1[0].origNum = malloc((strlen(origNum)+1) * sizeof(char));
				strcpy(currentBucketNode1 -> b1[0].origNum, origNum);

				currentBucketNode1 -> b1[0].extraCDR = malloc(sizeof(bucketNode2));

				currentBucketNode2 = currentBucketNode1 -> b1[0].extraCDR;

				currentBucketNode2 -> nextAvailablePos = 1;
				currentBucketNode2 -> next = NULL;
				currentBucketNode2 -> b2 = malloc(sizeof(bucket2));

				/***/
				currentBucketNode2 -> b2[0].cdr_uniq_id = malloc((strlen(cdr_uniq_id)+1) * sizeof(char));
				strcpy(currentBucketNode2 -> b2[0].cdr_uniq_id, cdr_uniq_id);

				currentBucketNode2 -> b2[0].destNum = malloc((strlen(destNum)+1) * sizeof(char));
				strcpy(currentBucketNode2 -> b2[0].destNum, destNum);

				currentBucketNode2 -> b2[0].date = malloc((strlen(date)+1) * sizeof(char));
				strcpy(currentBucketNode2 -> b2[0].date, date);

				currentBucketNode2 -> b2[0].time = malloc((strlen(time)+1) * sizeof(char));
				strcpy(currentBucketNode2 -> b2[0].time, time);

				currentBucketNode2 -> b2[0].duration = duration;

				currentBucketNode2 -> b2[0].type = type;

				currentBucketNode2 -> b2[0].tarrif = tarrif;

				currentBucketNode2 -> b2[0].fault_condition = fault_condition;
				/***/
			}
		}
	}	
	
}
