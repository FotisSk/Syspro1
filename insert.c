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
	int key;
	bucketNode1 *bn1;

	key = hashFunction(origNum, HT1numOfEntries);

	if(HT1[key].numOfNodes1 == 0)	//den exei ksanabei entry
	{
		bn1 = malloc(sizeof(bucketNode1));
		HT1[key].head1 = bn1;
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
	}
	
}
