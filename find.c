#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definition.h"
#include "insert.h"
#include "delete.h"
#include "find.h"

int findCaller(hashTable1 *HT1, int HT1numOfEntries, int scenario, char *origNum, char *time1, char *date1, char *time2, char *date2)
{
	int key, i, j, flag1, callsFound, fromTime, toTime;
	bucketNode1_caller *currentBucketNode1;
	bucketNode2_caller *currentBucketNode2;
	bucket2_caller *b2;
	dateStruct ds1, ds2;
	timeStruct ts1, ts2;

	key = hashFunction(origNum, HT1numOfEntries);

	flag1 = 0;
	currentBucketNode1 = HT1[key].head1;

	while(currentBucketNode1 && flag1 == 0)
	{
		i = 0;
		while(i<currentBucketNode1 -> nextAvailablePos && flag1 == 0)
		{
			if(strcmp(currentBucketNode1 -> b1[i].origNum, origNum) == 0)	//vrikame ton caller
			{
				if(currentBucketNode1 -> b1[i].numOfNodes2 == 0)
				{
					//printf("> Scenario %d\n", scenario);
					printf("No CDRs found  (%s)\n", origNum);
					//printf("|--------------------------------------------------------------------------------------------------------|\n");
					return -1;
				}
				flag1 = 1;
				currentBucketNode2 = currentBucketNode1 -> b1[i].extraCDR;
				callsFound = 0;

				/************* SCENARIOS *************/
				/* 0: CALLER                         */
				/* 1: CALLER TIME1 TIME2             */
				/* 2: CALLER DATE1 DATE2             */
				/* 3: CALLER TIME1 DATE1 TIME2 DATE2 */
				/*************************************/

				switch(scenario)
				{
					case 0:
						//printf("> Scenario 0\n");
						//printf("> Every call from '%s'\n", origNum);
						while(currentBucketNode2)	//psakse oli ti lista mexri to telos
						{
							for(j=0; j<currentBucketNode2 -> nextAvailablePos; j++)
							{
								b2 = currentBucketNode2 -> b2;
								callsFound++;
								printf("%s;%s;%s;%d/%d/%d;%dH:%dM;%d;%d;%d;%d\n", b2[j].cdr_uniq_id, origNum, b2[j].destNum, b2[j].date.day, b2[j].date.month, b2[j].date.year,
																			    b2[j].time.hours, b2[j].time.minutes, b2[j].duration, b2[j].type,
																			        b2[j].tarrif, b2[j].fault_condition);
							}
							currentBucketNode2 = currentBucketNode2 -> next;
							
						}
						if(callsFound == 0)
							printf("No CDRs found  (%s)\n", origNum);
						//printf("> Total of %d calls found.\n", callsFound);
						//printf("|--------------------------------------------------------------------------------------------------------|\n");
						break;

					case 1:
						//printf("> Scenario 1\n");
						ts1 = timeToIntegers(time1);
						ts2 = timeToIntegers(time2);

						if(ts2.totalMinutes < ts1.totalMinutes)
						{
							fromTime = ts2.totalMinutes;
							toTime = ts1.totalMinutes;
							//printf("> Every call from '%s' between %dH:%dM and %dH:%dM\n", origNum, ts2.hours, ts2.minutes, ts1.hours, ts1.minutes);
						}
						else if(ts1.totalMinutes <= ts2.totalMinutes)
						{
							fromTime = ts1.totalMinutes;
							toTime = ts2.totalMinutes;
							//printf("> Every call from '%s' between %dH:%dM and %dH:%dM\n", origNum, ts1.hours, ts1.minutes, ts2.hours, ts2.minutes);
						}
						while(currentBucketNode2)
						{
							b2 = currentBucketNode2 -> b2;
							for(j=0; j<currentBucketNode2 -> nextAvailablePos; j++)
							{
								if(b2[j].time.totalMinutes >= fromTime && b2[j].time.totalMinutes <= toTime)
								{
									callsFound++;
									printf("%s;%s;%s;%d/%d/%d;%dH:%dM;%d;%d;%d;%d\n", b2[j].cdr_uniq_id, origNum, b2[j].destNum, b2[j].date.day, b2[j].date.month, b2[j].date.year,
																			        b2[j].time.hours, b2[j].time.minutes, b2[j].duration, b2[j].type,
																			            b2[j].tarrif, b2[j].fault_condition);
								}
							}
							currentBucketNode2 = currentBucketNode2 -> next;
						}
						if(callsFound == 0)
							printf("No CDRs found  (%s)\n", origNum);
						//printf("> Total of %d calls found.\n", callsFound);
						//printf("|--------------------------------------------------------------------------------------------------------|\n");
						break;

					case 2:
						//printf("> Scenario 2\n");
						ds1 = dateToIntegers(date1);
						ds2 = dateToIntegers(date2);
						//printf("> Every call from '%s' between %d/%d/%d and %d/%d/%d\n", origNum, ds1.day, ds1.month, ds1.year, ds2.day, ds2.month, ds2.year);

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
								printf("%s;%s;%s;%d/%d/%d;%dH:%dM;%d;%d;%d;%d\n", b2[j].cdr_uniq_id, origNum, b2[j].destNum, b2[j].date.day, b2[j].date.month, b2[j].date.year,
																			    b2[j].time.hours, b2[j].time.minutes, b2[j].duration, b2[j].type, b2[j].tarrif, b2[j].fault_condition);
							}
							currentBucketNode2 = currentBucketNode2 -> next;
						}
						if(callsFound == 0)
							printf("No CDRs found  (%s)\n", origNum);
						//printf("> Total of %d calls found.\n", callsFound);
						//printf("|--------------------------------------------------------------------------------------------------------|\n");
						break;

					case 3:
						//printf("> Scenario 3\n");
						ds1 = dateToIntegers(date1);
						ds2 = dateToIntegers(date2);
						ts1 = timeToIntegers(time1);
						ts2 = timeToIntegers(time2);

						//printf("> Every call from '%s' between %dH:%dM %d/%d/%d and %dH:%dM %d/%d/%d\n", origNum, ts1.hours, ts1.minutes, ds1.day, ds1.month, ds1.year, ts2.hours, ts2.minutes, ds2.day, ds2.month, ds2.year);

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
								printf("%s;%s;%s;%d/%d/%d;%dH:%dM;%d;%d;%d;%d\n", b2[j].cdr_uniq_id, origNum, b2[j].destNum, b2[j].date.day, b2[j].date.month, b2[j].date.year,
																			    b2[j].time.hours, b2[j].time.minutes, b2[j].duration, b2[j].type, b2[j].tarrif, b2[j].fault_condition);
							}
							currentBucketNode2 = currentBucketNode2 -> next;
						}
						if(callsFound == 0)
							printf("No CDRs found  (%s)\n", origNum);
						//printf("> Total of %d calls found.\n", callsFound);
						//printf("|--------------------------------------------------------------------------------------------------------|\n");
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
		printf("No CDRs found  (%s)\n", origNum);
		//printf("> Scenario %d\n", scenario);
		//printf("Caller '%s' not found.\n", origNum);
		//printf("|--------------------------------------------------------------------------------------------------------|\n");
		return -1;
	}
}