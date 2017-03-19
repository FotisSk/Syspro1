#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definition.h"
#include "insert.h"
#include "delete.h"
#include "find.h"
#include "lookup.h"

int lookupCallee(hashTable2 *HT2, int HT2numOfEntries, int scenario, char *destNum, char *time1, char *date1, char *time2, char *date2)
{
	int key, i, j, flag1, callsFound, fromTime, toTime;
	bucketNode1_callee *currentBucketNode1;
	bucketNode2_callee *currentBucketNode2;
	bucket2_callee *b2;
	dateStruct ds1, ds2;
	timeStruct ts1, ts2;

	key = hashFunction(destNum, HT2numOfEntries);

	flag1 = 0;
	currentBucketNode1 = HT2[key].head2;

	while(currentBucketNode1 && flag1 == 0)
	{
		i = 0;
		while(i<currentBucketNode1 -> nextAvailablePos && flag1 == 0)
		{
			if(strcmp(currentBucketNode1 -> b1[i].destNum, destNum) == 0)	//vrikame ton callee
			{
				if(currentBucketNode1 -> b1[i].numOfNodes2 == 0)
				{
					printf("[ lookup %s ... ]\n", destNum);
					printf("No CDRs found\n\n");
					return -1;
				}
				flag1 = 1;
				currentBucketNode2 = currentBucketNode1 -> b1[i].extraCDR;
				callsFound = 0;

				/************* SCENARIOS *************/
				/* 0: CALLEE                         */
				/* 1: CALLEE TIME1 TIME2             */
				/* 2: CALLEE DATE1 DATE2             */
				/* 3: CALLEE TIME1 DATE1 TIME2 DATE2 */
				/*************************************/

				switch(scenario)
				{
					case 0:
						printf("[ lookup %s ]\n", destNum);
						while(currentBucketNode2)	//psakse oli ti lista mexri to telos
						{
							for(j=0; j<currentBucketNode2 -> nextAvailablePos; j++)
							{
								b2 = currentBucketNode2 -> b2;
								callsFound++;
								printf("%s;%s;%s;%d/%d/%d;%dH:%dM;%d;%d;%d;%d\n", b2[j].cdr_uniq_id, destNum, b2[j].origNum, b2[j].date.day, b2[j].date.month, b2[j].date.year,
																			    	b2[j].time.hours, b2[j].time.minutes, b2[j].duration, b2[j].type, b2[j].tarrif, b2[j].fault_condition);
							}
							currentBucketNode2 = currentBucketNode2 -> next;
							
						}
						if(callsFound == 0)
							printf("No CDRs found\n");

						break;

					case 1:
						ts1 = timeToIntegers(time1);
						ts2 = timeToIntegers(time2);

						if(ts2.totalMinutes < ts1.totalMinutes)
						{
							fromTime = ts2.totalMinutes;
							toTime = ts1.totalMinutes;
							printf("[ lookup %s %dH:%dM - %dH:%dM ]\n", destNum, ts2.hours, ts2.minutes, ts1.hours, ts1.minutes);
						}
						else if(ts1.totalMinutes <= ts2.totalMinutes)
						{
							fromTime = ts1.totalMinutes;
							toTime = ts2.totalMinutes;
							printf("[ lookup %s %dH:%dM - %dH:%dM ]\n", destNum, ts1.hours, ts1.minutes, ts2.hours, ts2.minutes);
						}
						while(currentBucketNode2)
						{
							b2 = currentBucketNode2 -> b2;
							for(j=0; j<currentBucketNode2 -> nextAvailablePos; j++)
							{
								if(b2[j].time.totalMinutes >= fromTime && b2[j].time.totalMinutes <= toTime)
								{
									callsFound++;
									printf("%s;%s;%s;%d/%d/%d;%dH:%dM;%d;%d;%d;%d\n", b2[j].cdr_uniq_id, destNum, b2[j].origNum, b2[j].date.day, b2[j].date.month, b2[j].date.year,
																			    		b2[j].time.hours, b2[j].time.minutes, b2[j].duration, b2[j].type, b2[j].tarrif, b2[j].fault_condition);
								}
							}
							currentBucketNode2 = currentBucketNode2 -> next;
						}
						if(callsFound == 0)
							printf("No CDRs found\n");
					
						break;

					case 2:
						ds1 = dateToIntegers(date1);
						ds2 = dateToIntegers(date2);
						printf("[ lookup %s %d/%d/%d - %d/%d/%d ]\n", destNum, ds1.day, ds1.month, ds1.year, ds2.day, ds2.month, ds2.year);

						if(ds1.year > ds2.year)
							return -1;
						else if(ds1.year == ds2.year)
						{
							if(ds1.month > ds2.month)
								return -1;
							else if(ds1.month == ds2.month)
							{
								if(ds1.day > ds2.day)
									return -1;	
							}	
						}
						while(currentBucketNode2)
						{
							b2 = currentBucketNode2 -> b2;
							for(j=0; j<currentBucketNode2 -> nextAvailablePos; j++)
							{
								if(b2[j].date.year < ds1.year || b2[j].date.year > ds2.year)
									continue;
								else if(b2[j].date.year == ds1.year)
								{
									if(b2[j].date.month < ds1.month)
										continue;
									else if(b2[j].date.month == ds1.month)
									{
										if(b2[j].date.day < ds1.day)
											continue;
									}
								}
								else if(b2[j].date.year == ds2.year)
								{
									if(b2[j].date.month > ds2.month)
										continue;
									else if(b2[j].date.month == ds2.month)
									{
										if(b2[j].date.day > ds2.day)
											continue;
									}
								}

								callsFound++;
								printf("%s;%s;%s;%d/%d/%d;%dH:%dM;%d;%d;%d;%d\n", b2[j].cdr_uniq_id, destNum, b2[j].origNum, b2[j].date.day, b2[j].date.month, b2[j].date.year,
																			    	b2[j].time.hours, b2[j].time.minutes, b2[j].duration, b2[j].type, b2[j].tarrif, b2[j].fault_condition);
							}
							currentBucketNode2 = currentBucketNode2 -> next;
						}
						if(callsFound == 0)
							printf("No CDRs found\n");
					
						break;

					case 3:
						ds1 = dateToIntegers(date1);
						ds2 = dateToIntegers(date2);
						ts1 = timeToIntegers(time1);
						ts2 = timeToIntegers(time2);

						printf("[ lookup %s %dH:%dM %d/%d/%d - %dH:%dM %d/%d/%d ]\n", destNum, ts1.hours, ts1.minutes, ds1.day, ds1.month, ds1.year, ts2.hours, ts2.minutes, ds2.day, ds2.month, ds2.year);

						if(ds1.year > ds2.year)
							return -1;
						else if(ds1.year == ds2.year)
						{
							if(ds1.month > ds2.month)
								return -1;
							else if(ds1.month == ds2.month)
							{
								if(ds1.day > ds2.day)
									return -1;	
							}	
						}

						while(currentBucketNode2)
						{
							b2 = currentBucketNode2 -> b2;
							for(j=0; j<currentBucketNode2 -> nextAvailablePos; j++)
							{
								if(b2[j].date.year < ds1.year || b2[j].date.year > ds2.year)
									continue;
								else if(b2[j].date.year == ds1.year)
								{
									if(b2[j].date.month < ds1.month)
										continue;
									else if(b2[j].date.month == ds1.month)
									{
										if(b2[j].date.day < ds1.day)
											continue;
										else if(b2[j].date.day == ds1.day)
										{
											if(b2[j].time.totalMinutes < ts1.totalMinutes)
												continue;
										}
									}
								}
								else if(b2[j].date.year == ds2.year)
								{
									if(b2[j].date.month > ds2.month)
										continue;
									else if(b2[j].date.month == ds2.month)
									{
										if(b2[j].date.day > ds2.day)
											continue;
										else if(b2[j].date.day == ds2.day)
										{
											if(b2[j].time.totalMinutes > ts2.totalMinutes)
												continue;
										}
									}
								}

								callsFound++;
								printf("%s;%s;%s;%d/%d/%d;%dH:%dM;%d;%d;%d;%d\n", b2[j].cdr_uniq_id, destNum, b2[j].origNum, b2[j].date.day, b2[j].date.month, b2[j].date.year,
																			    	b2[j].time.hours, b2[j].time.minutes, b2[j].duration, b2[j].type, b2[j].tarrif, b2[j].fault_condition);
							}
							currentBucketNode2 = currentBucketNode2 -> next;
						}
						if(callsFound == 0)
							printf("No CDRs found\n");
				
						break;
				}

			}
			else
				i++;
		}
		currentBucketNode1 = currentBucketNode1 -> next;
	}
	if(flag1 == 0)
	{
		printf("[ lookup %s ... ]\n", destNum);
		printf("No CDRs found\n\n");
		return -1;
	}
	printf("\n");
}