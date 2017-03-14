#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definition.h"
#include "insert.h"
#include "delete.h"
#include "find.h"
#include "lookup.h"
#include "indist1.h"

int insertToIndistList(hashTable1 *HT1, int HT1numOfEntries, hashTable2 *HT2, int HT2numOfEntries, char *phoneNum, indistList *headNode)
{
	int numOfNodes, i, j, k, l, m, key1, key2, flag, deny;
	subscriberNode *currentListNode, *previousListNode;
	bucketNode1_caller *currentBucketNode1_caller;
	bucketNode2_caller *currentBucketNode2_caller;
	bucketNode1_callee *currentBucketNode1_callee;
	bucketNode2_callee *currentBucketNode2_callee;

	previousListNode = NULL;
	currentListNode = headNode -> head;
	numOfNodes = headNode -> numOfNodes;
	for(i=0; i<numOfNodes; i++)
	{
		if(strcmp(phoneNum, currentListNode -> phoneNum) == 0)
			return -1;
		else
			currentListNode = currentListNode -> next;
	}
	
	deny = 0;
	currentListNode = headNode -> head;
	for(i=0; i<numOfNodes; i++)
	{
		key1 = hashFunction(phoneNum, HT1numOfEntries);
		currentBucketNode1_caller = HT1[key1].head1;
		flag = 0;
		while(currentBucketNode1_caller && flag == 0)
		{
			for(j=0; j<currentBucketNode1_caller -> nextAvailablePos; j++)
			{
				if(strcmp(phoneNum, currentBucketNode1_caller -> b1[j].origNum) == 0)
				{
					flag = 1;
					currentBucketNode2_caller = currentBucketNode1_caller -> b1[j].extraCDR;
					while(currentBucketNode2_caller && deny == 0)
					{
						for(k=0; k<currentBucketNode2_caller -> nextAvailablePos; k++)
						{
							if(strcmp(currentListNode -> phoneNum, currentBucketNode2_caller -> b2[k].destNum) == 0)
							{
								deny = 1;
								printf("Insert denied. '%s' has contacted '%s'\n", phoneNum, currentListNode -> phoneNum);
								break;
							}
						}
						currentBucketNode2_caller = currentBucketNode2_caller -> next;
					}
				}
				if(flag == 1)
					break;

			}//end for j

			currentBucketNode1_caller = currentBucketNode1_caller -> next;
		}


		if(deny == 0) //den vrike kati stis ekserxomenes, tora tha koitaksei tis eiserxomenes.
		{
			key2 = hashFunction(phoneNum, HT2numOfEntries);
			currentBucketNode1_callee = HT2[key2].head2;
			flag = 0;
			while(currentBucketNode1_callee && flag == 0)
			{
				for(l=0; l<currentBucketNode1_callee -> nextAvailablePos; l++)
				{
					if(strcmp(phoneNum, currentBucketNode1_callee -> b1[l].destNum) == 0)
					{
						flag = 1;
						currentBucketNode2_callee = currentBucketNode1_callee -> b1[l].extraCDR;
						while(currentBucketNode2_callee && flag == 0)
						{
							for(m=0; m<currentBucketNode2_callee -> nextAvailablePos; m++)
							{
								if(strcmp(currentListNode -> phoneNum, currentBucketNode2_callee -> b2[m].origNum) == 0)
								{
									deny = 1;
									printf(" '%s' has contacted '%s'\n", phoneNum, currentListNode -> phoneNum);
									break;
								}
							}
							currentBucketNode2_callee = currentBucketNode2_callee -> next;
						}
					}
					if(flag == 1)
						break;
				}
				currentBucketNode1_callee = currentBucketNode1_callee -> next;
			}
		}
		if(i != numOfNodes -1)
			currentListNode = currentListNode -> next;

	}//end for i
	if(deny == 0)
	{
		//tote mono kanei eisagogi sti lista ton sindromiti.
		if(headNode -> numOfNodes == 0)
		{
			//printf("\n");
			headNode -> head = malloc(sizeof(subscriberNode));
			headNode -> head -> phoneNum = malloc((strlen(phoneNum)+1) * sizeof(char));
			strcpy(headNode -> head -> phoneNum, phoneNum);
			headNode -> head -> next = NULL;
			headNode -> numOfNodes++;
		}
		else
		{
			currentListNode -> next = malloc(sizeof(subscriberNode));
			currentListNode -> next -> phoneNum = malloc((strlen(phoneNum)+1) * sizeof(char));
			strcpy(currentListNode -> next -> phoneNum, phoneNum);
			currentListNode -> next -> next = NULL;
			headNode -> numOfNodes++;
		}
	}
}

indistList* indist(hashTable1 *HT1, int HT1numOfEntries, hashTable2 *HT2, int HT2numOfEntries, char *caller1, char *caller2)
{
	int i, j, k, l, m, n, subCount, flag1, flag2, key, foundYou;
	char *phoneNum;
	indistList *headNode;
	bucketNode1_caller *currentBucketNode1_caller;
	bucketNode2_caller *currentBucketNode2_caller;
	bucketNode1_callee *currentBucketNode1_callee;
	bucketNode2_callee *currentBucketNode2_callee;

	headNode = malloc(sizeof(indistList));

	headNode -> caller1 = malloc((strlen(caller1)+1) * sizeof(char));
	strcpy(headNode -> caller1, caller1);

	headNode -> caller2 = malloc((strlen(caller2)+1) * sizeof(char));
	strcpy(headNode -> caller2, caller2);	

	headNode -> numOfNodes = 0;

	headNode -> head = NULL;

	//ksekiname psaksimo apo HT1
	for(i=0; i<HT1numOfEntries; i++)
	{
		currentBucketNode1_caller = HT1[i].head1;
		while(currentBucketNode1_caller)
		{
			for(j=0; j<currentBucketNode1_caller-> nextAvailablePos; j++)
			{
				phoneNum = malloc((strlen(currentBucketNode1_caller -> b1[j].origNum)+1) * sizeof(char));
				strcpy(phoneNum, currentBucketNode1_caller -> b1[j].origNum);

				currentBucketNode2_caller = currentBucketNode1_caller -> b1[j].extraCDR;
				subCount = 0;
				flag1 = 0;
				flag2 = 0;
				while(currentBucketNode2_caller && subCount < 2)
				{
					for(k=0; k<currentBucketNode2_caller -> nextAvailablePos; k++)
					{
						if( (strcmp(caller1, currentBucketNode2_caller -> b2[k].destNum) == 0) && flag1 == 0 )
						{
							subCount++;
							flag1 = 1;
							if(subCount == 2)
								break;
						}
						else if( (strcmp(caller2, currentBucketNode2_caller -> b2[k].destNum) == 0) && flag2 == 0 )
						{
							subCount++;
							flag2 = 1;
							if(subCount == 2)
								break;
						}
					}
					currentBucketNode2_caller = currentBucketNode2_caller -> next;
				}
				

				if(subCount < 2)	//tote subCount = 0 h 1. ara prepei na psaksoume kai to HT2
				{
					key = hashFunction(phoneNum, HT2numOfEntries);
					currentBucketNode1_callee = HT2[key].head2;
					foundYou = 0;
					while(currentBucketNode1_callee && subCount < 2)
					{	
						for(m=0; m<currentBucketNode1_callee -> nextAvailablePos; m++)
						{
							if(strcmp(phoneNum, currentBucketNode1_callee -> b1[m].destNum) == 0)
							{
								foundYou = 1;
								currentBucketNode2_callee = currentBucketNode1_callee -> b1[m].extraCDR;
								while(currentBucketNode2_callee && subCount < 2)
								{
									for(n=0; n<currentBucketNode2_callee -> nextAvailablePos; n++)
									{
										if( (strcmp(caller1, currentBucketNode2_callee -> b2[n].origNum) == 0) && flag1 == 0 )
										{
											subCount++;
											flag1 =1;
											if(subCount == 2)
												break;
										}
										else if( (strcmp(caller2, currentBucketNode2_callee -> b2[n].origNum) == 0) && flag2 == 0 )
										{
											subCount++;
											flag2 = 1;
											if(subCount == 2)
												break;
										}
									}
									currentBucketNode2_callee = currentBucketNode2_callee -> next;
								}
							}
							if(foundYou == 1)
								break;
						}
						if(foundYou == 1)
								break;

						currentBucketNode1_callee = currentBucketNode1_callee -> next;
					}


					if(subCount < 2)
					{
						if(flag1 == 0 && flag2 == 0)
						{
							//printf("Sub '%s' has not contacted either of sub1: '%s' or sub2: '%s'\n", phoneNum, caller1, caller2);
						}
						else if(flag1 == 0)
						{
							//printf("Sub '%s' has not contacted sub1: '%s'\n", phoneNum, caller1);
						}
						else
						{
							//printf("Sub '%s' has not contacted sub2: '%s'\n", phoneNum, caller2);
						}
					}
					else if(subCount == 2)
					{
						printf("Sub '%s' has  contacted both of sub1: '%s' and sub2: '%s'\n", phoneNum, caller1, caller2);
						insertToIndistList(HT1, HT1numOfEntries, HT2, HT2numOfEntries, phoneNum, headNode);
					}
				}
				else if(subCount == 2)
				{
					printf("Sub '%s' has  contacted both of sub1: '%s' and sub2: '%s'\n", phoneNum, caller1, caller2);
					insertToIndistList(HT1, HT1numOfEntries, HT2, HT2numOfEntries, phoneNum, headNode);
				}
				else
					printf("we fucked up bro\n");

				free(phoneNum);
				phoneNum = NULL;
			}//end for j
			currentBucketNode1_caller = currentBucketNode1_caller -> next;
		}
	}//end for i

	return headNode;

}