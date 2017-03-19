#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definition.h"
#include "insert.h"

int hashFunction(char *num, int HTnumOfEntries)
{
	int len, sum, i;
	
	sum = 33;
	len = strlen(num);
	for(i=0; i<len; i++)
	{
		sum = sum + (int)num[i];
	}
	return (sum % HTnumOfEntries);
}


dateStruct dateToIntegers(char *date)
{
	int dateInt, day, dayAndMonth, month, year;
	dateStruct ds;

	dateInt = atoi(date);

	day = dateInt / 1000000;
	dayAndMonth = dateInt / 10000;
	month = dayAndMonth % 100;
	year = dateInt % 10000;

	ds.day = day;
	ds.month = month;
	ds.year = year;

	return ds;
}

timeStruct timeToIntegers(char *time)
{
	int hours, minutes, totalMinutes;
	timeStruct ts;
	char *split;
	char timeParser[6];

	strcpy(timeParser, time);	//pername ston parser tin ora gia tin strtok

	split = strtok(timeParser, " :\r\n");
	hours = atoi(split);

	split = strtok(NULL, " :\r\n");
	minutes = atoi(split);

	totalMinutes = hours*60 + minutes;

	ts.hours = hours;
	ts.minutes = minutes;
	ts.totalMinutes = totalMinutes;

	return ts;
}

/*********************************************************************************************************************/
/****************************************************** CALLER ******************************************************/
int insertCaller(heap *hp, hashTable1 *HT1, int HT1numOfEntries, int bucket1_maxEntries, int bucket2_maxEntries,
 char *cdr_uniq_id, char *origNum, char *destNum, char *date, char *time,
  int duration, int type, int tarrif, int fault_condition, chargeListNode *chargeListHead)
{
	int key, nextAvailablePos1, nextAvailablePos2, numOfNodes1, i, j, flag1, flag2;
	bucketNode1_caller *bn1, *head1, *currentBucketNode1;
	bucketNode2_caller *currentBucketNode2;
	dateStruct ds;
	timeStruct ts;

	ds = dateToIntegers(date);
	ts = timeToIntegers(time);
	key = hashFunction(origNum, HT1numOfEntries);

	if(HT1[key].numOfNodes1 == 0)	//den exei ksanabei entry
	{
		bn1 = malloc(sizeof(bucketNode1_caller));
		HT1[key].head1 = bn1;
		HT1[key].numOfNodes1++;

		bn1 -> nextAvailablePos = 1;	//1 kai oxi 0 giati vazo entry sto bucket parallila
		bn1 -> next  = NULL;
		bn1 -> b1 = malloc(bucket1_maxEntries * sizeof(bucket1_caller));
		bn1 -> b1[0].origNum = malloc((strlen(origNum)+1) * sizeof(char));
		strcpy(bn1 -> b1[0].origNum, origNum);
		bn1 -> b1[0].numOfNodes2 = 1;
		bn1 -> b1[0].heapPtr = NULL;

		insertNode(hp, bn1, 0, HT1, HT1numOfEntries, origNum, duration, type, tarrif, fault_condition, chargeListHead);

		bn1 -> b1[0].extraCDR = malloc(sizeof(bucketNode2_caller));
		bn1 -> b1[0].extraCDR -> nextAvailablePos = 1;	//1 kai oxi 0 giati vazo entry sto bucket parallila
		bn1 -> b1[0].extraCDR -> next = NULL;
		bn1 -> b1[0].extraCDR -> b2 = malloc(bucket2_maxEntries * sizeof(bucket2_caller));

		bn1 -> b1[0].extraCDR -> b2[0].cdr_uniq_id = malloc((strlen(cdr_uniq_id)+1) * sizeof(char));
		strcpy(bn1 -> b1[0].extraCDR -> b2[0].cdr_uniq_id, cdr_uniq_id);

		bn1 -> b1[0].extraCDR -> b2[0].destNum = malloc((strlen(destNum)+1) * sizeof(char));
		strcpy(bn1 -> b1[0].extraCDR -> b2[0].destNum, destNum);

		bn1 -> b1[0].extraCDR -> b2[0].date = ds;

		bn1 -> b1[0].extraCDR -> b2[0].time = ts;

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
		for(i=0; i<numOfNodes1; i++)	//anazitisi an iparxei idi to origNum.
		{
			for(j=0; j<currentBucketNode1 -> nextAvailablePos; j++)
			{
				if(strcmp(currentBucketNode1 -> b1[j].origNum, origNum) == 0)	//an vreis to tilefono tote pigaine na deis ama xorane ta kainourgia extraCDR.
				{
					updateNode(hp, currentBucketNode1, j, duration, type, tarrif, fault_condition, chargeListHead);
					if(currentBucketNode1 -> b1[j].extraCDR)	//an den einai null to extraCDR apo deletes.
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

							currentBucketNode2 -> b2[nextAvailablePos2].date = ds;

							currentBucketNode2 -> b2[nextAvailablePos2].time = ts;

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
							currentBucketNode2 -> next = malloc(sizeof(bucketNode2_caller));
							currentBucketNode2 = currentBucketNode2 -> next;

							currentBucketNode1 -> b1[j].numOfNodes2++;

							currentBucketNode2 -> nextAvailablePos = 1;	//1 kai oxi 0 giati tha valo parallila mesa tin eggrafi
							currentBucketNode2 -> next = NULL;
							currentBucketNode2 -> b2 = malloc(bucket2_maxEntries * sizeof(bucket2_caller));


							/***/
							currentBucketNode2 -> b2[0].cdr_uniq_id = malloc((strlen(cdr_uniq_id)+1) * sizeof(char));
							strcpy(currentBucketNode2 -> b2[0].cdr_uniq_id, cdr_uniq_id);

							currentBucketNode2 -> b2[0].destNum = malloc((strlen(destNum)+1) * sizeof(char));
							strcpy(currentBucketNode2 -> b2[0].destNum, destNum);

							currentBucketNode2 -> b2[0].date = ds;

							currentBucketNode2 -> b2[0].time = ts;

							currentBucketNode2 -> b2[0].duration = duration;

							currentBucketNode2 -> b2[0].type = type;

							currentBucketNode2 -> b2[0].tarrif = tarrif;

							currentBucketNode2 -> b2[0].fault_condition = fault_condition;
							/***/
							return 1;
						}
					}
					else	//an einai NULL to extraCDR
					{
						currentBucketNode1 -> b1[j].extraCDR = malloc(sizeof(bucketNode2_caller));
						currentBucketNode1 -> b1[j].numOfNodes2++;	//diladi ginetai 1
						currentBucketNode2 = currentBucketNode1 -> b1[j].extraCDR;

						currentBucketNode2 -> nextAvailablePos = 1;
						currentBucketNode2 -> next = NULL;
						currentBucketNode2 -> b2 = malloc(bucket2_maxEntries * sizeof(bucket2_caller));

						/***/
						currentBucketNode2 -> b2[0].cdr_uniq_id = malloc((strlen(cdr_uniq_id)+1) * sizeof(char));
						strcpy(currentBucketNode2 -> b2[0].cdr_uniq_id, cdr_uniq_id);

						currentBucketNode2 -> b2[0].destNum = malloc((strlen(destNum)+1) * sizeof(char));
						strcpy(currentBucketNode2 -> b2[0].destNum, destNum);

						currentBucketNode2 -> b2[0].date = ds;

						currentBucketNode2 -> b2[0].time = ts;

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

				currentBucketNode1 -> b1[nextAvailablePos1].extraCDR = malloc(sizeof(bucketNode2_caller));
				currentBucketNode1 -> b1[nextAvailablePos1].numOfNodes2 = 1;
				currentBucketNode1 -> b1[nextAvailablePos1].heapPtr = NULL;

				insertNode(hp, currentBucketNode1, nextAvailablePos1, HT1, HT1numOfEntries, origNum, duration, type, tarrif, fault_condition, chargeListHead);

				currentBucketNode1 -> nextAvailablePos++;

				currentBucketNode2 = currentBucketNode1 -> b1[nextAvailablePos1].extraCDR;
				currentBucketNode2 -> nextAvailablePos = 1;
				currentBucketNode2 -> next = NULL;
				currentBucketNode2 -> b2 = malloc(bucket2_maxEntries * sizeof(bucket2_caller));

				/***/
				currentBucketNode2 -> b2[0].cdr_uniq_id = malloc((strlen(cdr_uniq_id)+1) * sizeof(char));
				strcpy(currentBucketNode2 -> b2[0].cdr_uniq_id, cdr_uniq_id);

				currentBucketNode2 -> b2[0].destNum = malloc((strlen(destNum)+1) * sizeof(char));
				strcpy(currentBucketNode2 -> b2[0].destNum, destNum);

				currentBucketNode2 -> b2[0].date = ds;

				currentBucketNode2 -> b2[0].time = ts;

				currentBucketNode2 -> b2[0].duration = duration;

				currentBucketNode2 -> b2[0].type = type;

				currentBucketNode2 -> b2[0].tarrif = tarrif;

				currentBucketNode2 -> b2[0].fault_condition = fault_condition;
				/***/
			}
			else	//an de xoraei to entry sto teleutaio bucket1
			{
				currentBucketNode1 -> next = malloc(sizeof(bucketNode1_caller));
				HT1[key].numOfNodes1++;
				currentBucketNode1 = currentBucketNode1 -> next;
				
				currentBucketNode1 -> nextAvailablePos = 1;
				currentBucketNode1 -> next = NULL;
				currentBucketNode1 -> b1 = malloc(bucket1_maxEntries * sizeof(bucket1_caller));

				currentBucketNode1 -> b1[0].origNum = malloc((strlen(origNum)+1) * sizeof(char));
				strcpy(currentBucketNode1 -> b1[0].origNum, origNum);

				currentBucketNode1 -> b1[0].extraCDR = malloc(sizeof(bucketNode2_caller));
				currentBucketNode1 -> b1[0].numOfNodes2 = 1;
				currentBucketNode1 -> b1[0].heapPtr = NULL;

				insertNode(hp, currentBucketNode1, 0, HT1, HT1numOfEntries, origNum, duration, type, tarrif, fault_condition, chargeListHead);

				currentBucketNode2 = currentBucketNode1 -> b1[0].extraCDR;

				currentBucketNode2 -> nextAvailablePos = 1;
				currentBucketNode2 -> next = NULL;
				currentBucketNode2 -> b2 = malloc(bucket1_maxEntries * sizeof(bucket2_caller));

				/***/
				currentBucketNode2 -> b2[0].cdr_uniq_id = malloc((strlen(cdr_uniq_id)+1) * sizeof(char));
				strcpy(currentBucketNode2 -> b2[0].cdr_uniq_id, cdr_uniq_id);

				currentBucketNode2 -> b2[0].destNum = malloc((strlen(destNum)+1) * sizeof(char));
				strcpy(currentBucketNode2 -> b2[0].destNum, destNum);

				currentBucketNode2 -> b2[0].date = ds;

				currentBucketNode2 -> b2[0].time = ts;

				currentBucketNode2 -> b2[0].duration = duration;

				currentBucketNode2 -> b2[0].type = type;

				currentBucketNode2 -> b2[0].tarrif = tarrif;

				currentBucketNode2 -> b2[0].fault_condition = fault_condition;
				/***/

			}
		}
	}	
	
}


/*************************************************************************************************************************/
/******************************************************* HEAP ***********************************************************/
int insertNode(heap *hp, bucketNode1_caller *bucketNode1_child, int pos_child, hashTable1 *HT1, int HT1numOfEntries,
				 char *origNum, int duration, int type, int tarrif, int fault_condition, chargeListNode *chargeListHead)
{
	int nodeToInsert, fatherNodeNum, i, j, fatherFound, pos_father;
	heapNode *hnode, *childPtr;
	chargeListNode *currentChargeListNode;
	bucketNode1_caller *currentBucketNode1, *bucketNode1_father;

	
	nodeToInsert = hp -> numOfNodes + 1;

	//dimiourgise ton komvo kai dostou times
	hnode = malloc(sizeof(heapNode));
	hnode -> nodeNum = nodeToInsert;
	hnode -> subscriber = malloc((strlen(origNum)+1) * sizeof(char));
	strcpy(hnode -> subscriber, origNum);

	if(fault_condition >= 200 && fault_condition < 300)
	{
		currentChargeListNode = chargeListHead;
		while(currentChargeListNode)
		{
			if(currentChargeListNode -> c.type == type && currentChargeListNode -> c.tarrif == tarrif)
			{
				if(type == 0 && tarrif == 0)
				{
					hnode -> amount = currentChargeListNode -> c.cost;
					break;
				}
				else
				{
					hnode -> amount = (currentChargeListNode -> c.cost) * (double)duration;
					break;
				}
			}
				
			currentChargeListNode = currentChargeListNode -> next;
		}
	}
	else
		hnode -> amount = 0;

	hnode -> bucketNodePtr = bucketNode1_child;	
	hnode -> bucketPos = pos_child;
	hnode -> leftChild = NULL;
	hnode -> rightChild = NULL;
	hnode -> inTopKList = 0;

	bucketNode1_child -> b1[pos_child].heapPtr = hnode;

	//hnode -> father = ?
	if(nodeToInsert != 1)
	{
		fatherNodeNum = nodeToInsert / 2;
		fatherFound = 0;
		pos_father = 0;
		for(i=0; i<HT1numOfEntries; i++)
		{
			currentBucketNode1 = HT1[i].head1;
			while(currentBucketNode1 && fatherFound == 0)
			{
				for(j=0; j<currentBucketNode1 -> nextAvailablePos; j++)
				{
					if(currentBucketNode1 -> b1[j].heapPtr -> nodeNum == fatherNodeNum)
					{
						fatherFound = 1;
						bucketNode1_father = currentBucketNode1;
						pos_father = j;
						break;
					}
				}
				if(fatherFound == 0)
					currentBucketNode1 = currentBucketNode1 -> next;
			}
			if(fatherFound == 1)
				break;
		}
		hnode -> father = bucketNode1_father -> b1[pos_father].heapPtr;
		if(nodeToInsert % 2 == 0)
			hnode -> father -> leftChild = hnode;
		else
			hnode -> father -> rightChild = hnode;
		
		hp -> numOfNodes++;
		childPtr = hnode;
		//i eisagogi tou komvou teleiose edo. tora prepei na doume an diatireitai i idiotita tou max heap
		heapify(hp, childPtr);
	}
	else	//nodeToInsert = 1;
	{
		hp -> head = hnode;
		hnode -> father = NULL;
		hp -> numOfNodes++;	
	}

	return 1;
}


int updateNode(heap *hp, bucketNode1_caller *currentBucketNode1, int pos_child, int duration, int type, int tarrif, int fault_condition, chargeListNode *chargeListHead)
{
	heapNode *childPtr;
	chargeListNode *currentChargeListNode;

	childPtr = currentBucketNode1 -> b1[pos_child].heapPtr;
	if(fault_condition >= 200 && fault_condition < 300)
	{
		currentChargeListNode = chargeListHead;
		while(currentChargeListNode)
		{
			if(currentChargeListNode -> c.type == type && currentChargeListNode -> c.tarrif == tarrif)
			{
				if(type == 0 && tarrif == 0)
				{
					childPtr -> amount += currentChargeListNode -> c.cost;
					break;
				}
				else
				{
					childPtr -> amount = (childPtr -> amount) + (currentChargeListNode -> c.cost) * (double)duration;
					break;
				}
			}
				
			currentChargeListNode = currentChargeListNode -> next;
		}
	}
	else
		childPtr -> amount += 0;

	if(childPtr -> nodeNum != 1)
		heapify(hp, childPtr);//, currentBucketNode1, pos_child);
}

int heapify(heap *hp, heapNode *childPtr)//, bucketNode1_caller *bucketNode1_child, int pos_child)
{
	int nodeNumTemp, bucketPosTemp;
	double amountTemp;
	char *subscriberTemp;
	heapNode *fatherPtr;
	bucketNode1_caller *bucketNodePtrTemp;

	fatherPtr = childPtr -> father;

	while(childPtr -> amount > fatherPtr -> amount)	//kane swap
	{
		//bucketPos
		bucketPosTemp = fatherPtr -> bucketPos;
		fatherPtr -> bucketPos = childPtr -> bucketPos;
		childPtr -> bucketPos = bucketPosTemp;
		//amount
		amountTemp = fatherPtr -> amount;
		fatherPtr -> amount = childPtr -> amount;
		childPtr -> amount = amountTemp;
		//subscriber
		subscriberTemp = malloc((strlen(fatherPtr -> subscriber)+1) * sizeof(char));
		strcpy(subscriberTemp, fatherPtr -> subscriber);

		free(fatherPtr -> subscriber);
		fatherPtr -> subscriber = malloc((strlen(childPtr -> subscriber)+1) * sizeof(char));
		strcpy(fatherPtr -> subscriber, childPtr -> subscriber);

		free(childPtr -> subscriber);
		childPtr -> subscriber = malloc((strlen(subscriberTemp)+1) * sizeof(char));
		strcpy(childPtr -> subscriber, subscriberTemp);

		free(subscriberTemp);
		subscriberTemp = NULL;

		//pointers
		childPtr -> bucketNodePtr -> b1[fatherPtr -> bucketPos].heapPtr = fatherPtr;
		bucketNodePtrTemp = childPtr -> bucketNodePtr;
		childPtr -> bucketNodePtr = fatherPtr -> bucketNodePtr;
		fatherPtr -> bucketNodePtr -> b1[childPtr -> bucketPos].heapPtr = childPtr;
		fatherPtr -> bucketNodePtr = bucketNodePtrTemp;
		//
		if(!fatherPtr -> father) //an o pateras tou patera einai null, tote allaksame ton headNode to heap mas.
		{
			hp -> head = fatherPtr;
			return 2;
		}
		//go next
		childPtr = childPtr -> father;
		fatherPtr = fatherPtr -> father;
	}
	return 1;
}


/*********************************************************************************************************************/
/****************************************************** CALLEE ******************************************************/
int insertCallee(hashTable2 *HT2, int HT2numOfEntries, int bucket1_maxEntries, int bucket2_maxEntries,
 char *cdr_uniq_id, char *origNum, char *destNum, char *date, char *time,
  int duration, int type, int tarrif, int fault_condition)
{
	int key, nextAvailablePos1, nextAvailablePos2, numOfNodes2, i, j, flag1, flag2;
	bucketNode1_callee *bn2, *head2, *currentBucketNode1;
	bucketNode2_callee *currentBucketNode2;
	dateStruct ds;
	timeStruct ts;

	ds = dateToIntegers(date);
	ts = timeToIntegers(time);
	key = hashFunction(destNum, HT2numOfEntries);

	if(HT2[key].numOfNodes2 == 0)	//den exei ksanabei entry
	{
		bn2 = malloc(sizeof(bucketNode1_callee));
		HT2[key].head2 = bn2;
		HT2[key].numOfNodes2++;

		bn2 -> nextAvailablePos = 1;	//1 kai oxi 0 giati vazo entry sto bucket parallila
		bn2 -> next  = NULL;
		bn2 -> b1 = malloc(bucket1_maxEntries * sizeof(bucket1_callee));
		bn2 -> b1[0].destNum = malloc((strlen(destNum)+1) * sizeof(char));
		strcpy(bn2 -> b1[0].destNum, destNum);
		bn2 -> b1[0].numOfNodes2 = 1;

		bn2 -> b1[0].extraCDR = malloc(sizeof(bucketNode2_callee));
		bn2 -> b1[0].extraCDR -> nextAvailablePos = 1;	//1 kai oxi 0 giati vazo entry sto bucket parallila
		bn2 -> b1[0].extraCDR -> next = NULL;
		bn2 -> b1[0].extraCDR -> b2 = malloc(bucket2_maxEntries * sizeof(bucket2_callee));

		bn2 -> b1[0].extraCDR -> b2[0].cdr_uniq_id = malloc((strlen(cdr_uniq_id)+1) * sizeof(char));
		strcpy(bn2 -> b1[0].extraCDR -> b2[0].cdr_uniq_id, cdr_uniq_id);

		bn2 -> b1[0].extraCDR -> b2[0].origNum = malloc((strlen(origNum)+1) * sizeof(char));
		strcpy(bn2 -> b1[0].extraCDR -> b2[0].origNum, origNum);

		bn2 -> b1[0].extraCDR -> b2[0].date = ds;

		bn2 -> b1[0].extraCDR -> b2[0].time = ts;

		bn2 -> b1[0].extraCDR -> b2[0].duration = duration;
		bn2 -> b1[0].extraCDR -> b2[0].type = type;
		bn2 -> b1[0].extraCDR -> b2[0].tarrif = tarrif;
		bn2 -> b1[0].extraCDR -> b2[0].fault_condition = fault_condition;
		return 1;
	}
	else	//to HT2[key].head den einai NULL, iparxoun idi entries.
	{
		numOfNodes2 = HT2[key].numOfNodes2;
		head2 = HT2[key].head2;
		currentBucketNode1 = head2;
		flag1 = 0;
		for(i=0; i<numOfNodes2; i++)	//anazitisi an iparxei idi to destNum.
		{
			for(j=0; j<currentBucketNode1 -> nextAvailablePos; j++)
			{
				if(strcmp(currentBucketNode1 -> b1[j].destNum, destNum) == 0)	//an vreis to tilefono tote pigaine na deis ama xorane ta kainourgia extraCDR.
				{
					if(currentBucketNode1 -> b1[j].extraCDR)	//an den einai null to extraCDR apo deletes.
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

							currentBucketNode2 -> b2[nextAvailablePos2].origNum = malloc((strlen(origNum)+1) * sizeof(char));
							strcpy(currentBucketNode2 -> b2[nextAvailablePos2].origNum, origNum);

							currentBucketNode2 -> b2[nextAvailablePos2].date = ds;

							currentBucketNode2 -> b2[nextAvailablePos2].time = ts;

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
							currentBucketNode2 -> next = malloc(sizeof(bucketNode2_callee));
							currentBucketNode2 = currentBucketNode2 -> next;

							currentBucketNode1 -> b1[j].numOfNodes2++;

							currentBucketNode2 -> nextAvailablePos = 1;	//1 kai oxi 0 giati tha valo parallila mesa tin eggrafi
							currentBucketNode2 -> next = NULL;
							currentBucketNode2 -> b2 = malloc(bucket2_maxEntries * sizeof(bucket2_callee));


							/***/
							currentBucketNode2 -> b2[0].cdr_uniq_id = malloc((strlen(cdr_uniq_id)+1) * sizeof(char));
							strcpy(currentBucketNode2 -> b2[0].cdr_uniq_id, cdr_uniq_id);

							currentBucketNode2 -> b2[0].origNum = malloc((strlen(origNum)+1) * sizeof(char));
							strcpy(currentBucketNode2 -> b2[0].origNum, origNum);

							currentBucketNode2 -> b2[0].date = ds;

							currentBucketNode2 -> b2[0].time = ts;

							currentBucketNode2 -> b2[0].duration = duration;

							currentBucketNode2 -> b2[0].type = type;

							currentBucketNode2 -> b2[0].tarrif = tarrif;

							currentBucketNode2 -> b2[0].fault_condition = fault_condition;
							/***/
							return 1;
						}
					}
					else	//an einai NULL to extraCDR
					{
						currentBucketNode1 -> b1[j].extraCDR = malloc(sizeof(bucketNode2_callee));
						currentBucketNode1 -> b1[j].numOfNodes2++;	//diladi ginetai 1
						currentBucketNode2 = currentBucketNode1 -> b1[j].extraCDR;

						currentBucketNode2 -> nextAvailablePos = 1;
						currentBucketNode2 -> next = NULL;
						currentBucketNode2 -> b2 = malloc(bucket2_maxEntries * sizeof(bucket2_callee));

						/***/
						currentBucketNode2 -> b2[0].cdr_uniq_id = malloc((strlen(cdr_uniq_id)+1) * sizeof(char));
						strcpy(currentBucketNode2 -> b2[0].cdr_uniq_id, cdr_uniq_id);

						currentBucketNode2 -> b2[0].origNum = malloc((strlen(origNum)+1) * sizeof(char));
						strcpy(currentBucketNode2 -> b2[0].origNum, origNum);

						currentBucketNode2 -> b2[0].date = ds;

						currentBucketNode2 -> b2[0].time = ts;

						currentBucketNode2 -> b2[0].duration = duration;

						currentBucketNode2 -> b2[0].type = type;

						currentBucketNode2 -> b2[0].tarrif = tarrif;
						currentBucketNode2 -> b2[0].fault_condition = fault_condition;
						/***/
						return 1;
					}
				}
			}
			if(i != numOfNodes2-1)	//oste stin teleutaia epanalipsi na min deiksei NULL
				currentBucketNode1 = currentBucketNode1 -> next;
		}

		if(flag1 == 0)	//de vrike idio destNum
		{
			nextAvailablePos1 = currentBucketNode1 -> nextAvailablePos;
			if(nextAvailablePos1 < bucket1_maxEntries)	//an xoraei to entry sto teleutaio bucket1
			{
				currentBucketNode1 -> b1[nextAvailablePos1].destNum = malloc((strlen(destNum)+1) * sizeof(char));
				strcpy(currentBucketNode1 -> b1[nextAvailablePos1].destNum, destNum);

				currentBucketNode1 -> b1[nextAvailablePos1].extraCDR = malloc(sizeof(bucketNode2_callee));
				currentBucketNode1 -> b1[nextAvailablePos1].numOfNodes2 = 1;

				currentBucketNode1 -> nextAvailablePos++;

				currentBucketNode2 = currentBucketNode1 -> b1[nextAvailablePos1].extraCDR;
				currentBucketNode2 -> nextAvailablePos = 1;
				currentBucketNode2 -> next = NULL;
				currentBucketNode2 -> b2 = malloc(bucket2_maxEntries * sizeof(bucket2_callee));

				/***/
				currentBucketNode2 -> b2[0].cdr_uniq_id = malloc((strlen(cdr_uniq_id)+1) * sizeof(char));
				strcpy(currentBucketNode2 -> b2[0].cdr_uniq_id, cdr_uniq_id);

				currentBucketNode2 -> b2[0].origNum = malloc((strlen(origNum)+1) * sizeof(char));
				strcpy(currentBucketNode2 -> b2[0].origNum, origNum);

				currentBucketNode2 -> b2[0].date = ds;

				currentBucketNode2 -> b2[0].time = ts;

				currentBucketNode2 -> b2[0].duration = duration;

				currentBucketNode2 -> b2[0].type = type;

				currentBucketNode2 -> b2[0].tarrif = tarrif;

				currentBucketNode2 -> b2[0].fault_condition = fault_condition;
				/***/
			}
			else	//an de xoraei to entry sto teleutaio bucket1
			{
				currentBucketNode1 -> next = malloc(sizeof(bucketNode1_callee));
				HT2[key].numOfNodes2++;
				currentBucketNode1 = currentBucketNode1 -> next;
				
				currentBucketNode1 -> nextAvailablePos = 1;
				currentBucketNode1 -> next = NULL;
				currentBucketNode1 -> b1 = malloc(bucket1_maxEntries * sizeof(bucket1_callee));

				currentBucketNode1 -> b1[0].destNum = malloc((strlen(destNum)+1) * sizeof(char));
				strcpy(currentBucketNode1 -> b1[0].destNum, destNum);

				currentBucketNode1 -> b1[0].extraCDR = malloc(sizeof(bucketNode2_callee));
				currentBucketNode1 -> b1[0].numOfNodes2 = 1;

				currentBucketNode2 = currentBucketNode1 -> b1[0].extraCDR;

				currentBucketNode2 -> nextAvailablePos = 1;
				currentBucketNode2 -> next = NULL;
				currentBucketNode2 -> b2 = malloc(bucket2_maxEntries * sizeof(bucket2_callee));

				/***/
				currentBucketNode2 -> b2[0].cdr_uniq_id = malloc((strlen(cdr_uniq_id)+1) * sizeof(char));
				strcpy(currentBucketNode2 -> b2[0].cdr_uniq_id, cdr_uniq_id);

				currentBucketNode2 -> b2[0].origNum = malloc((strlen(origNum)+1) * sizeof(char));
				strcpy(currentBucketNode2 -> b2[0].origNum, origNum);

				currentBucketNode2 -> b2[0].date = ds;

				currentBucketNode2 -> b2[0].time = ts;

				currentBucketNode2 -> b2[0].duration = duration;

				currentBucketNode2 -> b2[0].type = type;

				currentBucketNode2 -> b2[0].tarrif = tarrif;

				currentBucketNode2 -> b2[0].fault_condition = fault_condition;
				/***/
			}
		}
	}	
	
}