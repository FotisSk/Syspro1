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
int insertCaller(hashTable1 *HT1, int HT1numOfEntries, int bucket1_maxEntries, int bucket2_maxEntries,
 char *cdr_uniq_id, char *origNum, char *destNum, char *date, char *time,
  int duration, int type, int tarrif, int fault_condition)
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
		//nextAvailablePos1 = head1 -> nextAvailablePos;
		for(i=0; i<numOfNodes1; i++)	//anazitisi an iparxei idi to origNum.
		{
			for(j=0; j<currentBucketNode1 -> nextAvailablePos; j++)
			{
				if(strcmp(currentBucketNode1 -> b1[j].origNum, origNum) == 0)	//an vreis to tilefono tote pigaine na deis ama xorane ta kainourgia extraCDR.
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