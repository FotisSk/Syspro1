#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definition.h"
#include "insert.h"
#include "delete.h"
#include "find.h"
#include "lookup.h"
#include "indist1.h"

int insertToIndistListCaller(hashTable1 *HT1, int HT1numOfEntries, hashTable2 *HT2, int HT2numOfEntries, char *phoneNum, indistList *headNode, bucketNode2_caller *cbn2_caller)
{
	int numOfNodes, i, j, k, l, m, key, foundYou, flag, deny;
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
		{
			return -1;
		}
		else
			currentListNode = currentListNode -> next;
	}

	//gia eiserxomenes
	key = hashFunction(phoneNum, HT2numOfEntries);
	currentBucketNode1_callee = HT2[key].head2;
	foundYou = 0;
	while(currentBucketNode1_callee && foundYou == 0)	
	{
		for(j=0; j<currentBucketNode1_callee -> nextAvailablePos; j++)
		{
			if(strcmp(phoneNum, currentBucketNode1_callee -> b1[j].destNum) == 0)
			{
				foundYou = 1;
				currentBucketNode2_callee = currentBucketNode1_callee -> b1[j].extraCDR;
				break;
			}
		}
		currentBucketNode1_callee = currentBucketNode1_callee -> next;
	}	

	deny = 0;
	currentListNode = headNode -> head;
	for(i=0; i<numOfNodes; i++)
	{
		flag == 0;
		currentBucketNode2_caller = cbn2_caller;
		while(currentBucketNode2_caller && flag == 0)
		{
			for(k=0; k<currentBucketNode2_caller -> nextAvailablePos; k++)
			{
				if(strcmp(currentListNode -> phoneNum, currentBucketNode2_caller -> b2[k].destNum) == 0)
				{
					deny = 1;
					flag = 1;
					if(previousListNode)
					{
						previousListNode -> next = currentListNode -> next;
						free(currentListNode -> phoneNum);
						currentListNode -> phoneNum = NULL;
						free(currentListNode);
						currentListNode = previousListNode -> next;
						headNode -> numOfNodes--;
					}
					else
					{
						headNode -> head = currentListNode -> next;
						free(currentListNode -> phoneNum);
						currentListNode -> phoneNum = NULL;
						free(currentListNode);
						currentListNode = headNode -> head;
						headNode -> numOfNodes--;
					}
					break;
				}
			}
			currentBucketNode2_caller = currentBucketNode2_caller -> next;
		}
		
		if(flag == 0) //den vrike kati stis ekserxomenes, tora tha koitaksei tis eiserxomenes.
		{
			while(currentBucketNode2_callee && flag == 0)
			{
				for(m=0; m<currentBucketNode2_callee -> nextAvailablePos; m++)
				{
					if(strcmp(currentListNode -> phoneNum, currentBucketNode2_callee -> b2[m].origNum) == 0)
					{
						deny = 1;
						flag = 1;
						if(previousListNode)
						{
							previousListNode -> next = currentListNode -> next;
							free(currentListNode -> phoneNum);
							currentListNode -> phoneNum = NULL;
							free(currentListNode);
							currentListNode = previousListNode -> next;
							headNode -> numOfNodes--;
						}
						else
						{
							headNode -> head = currentListNode -> next;
							free(currentListNode -> phoneNum);
							currentListNode -> phoneNum = NULL;
							free(currentListNode);
							currentListNode = headNode -> head;
							headNode -> numOfNodes--;
						}
						break;
					}
				}
				currentBucketNode2_callee = currentBucketNode2_callee -> next;
			}	
		}
		if(i != numOfNodes -1 && flag == 0)
		{
			previousListNode = currentListNode;
			currentListNode = currentListNode -> next;
		}
	}//end for i

	if(deny == 0)
	{
		//tote mono kanei eisagogi sti lista ton sindromiti.
		if(headNode -> numOfNodes == 0)
		{
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

int insertToIndistListCallee(hashTable1 *HT1, int HT1numOfEntries, hashTable2 *HT2, int HT2numOfEntries, char *phoneNum, indistList *headNode, bucketNode2_callee *cbn2_callee)
{
	int numOfNodes, i, j, k, l, m, key, foundYou, flag, deny;
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
		{
			return -1;
		}
		else
			currentListNode = currentListNode -> next;
	}
	deny = 0;
	currentListNode = headNode -> head;
	for(i=0; i<numOfNodes; i++)
	{
		flag = 0;
		currentBucketNode2_callee = cbn2_callee;
		while(currentBucketNode2_callee && flag == 0)
		{
			for(k=0; k<currentBucketNode2_callee -> nextAvailablePos; k++)
			{
				if(strcmp(currentListNode -> phoneNum, currentBucketNode2_callee -> b2[k].origNum) == 0)
				{
					deny = 1;
					flag = 1;
					if(previousListNode)
					{
						previousListNode -> next = currentListNode -> next;
						free(currentListNode -> phoneNum);
						currentListNode -> phoneNum = NULL;
						free(currentListNode);
						currentListNode = previousListNode -> next;
						headNode -> numOfNodes--;
					}
					else
					{
						headNode -> head = currentListNode -> next;
						free(currentListNode -> phoneNum);
						currentListNode -> phoneNum = NULL;
						free(currentListNode);
						currentListNode = headNode -> head;
						headNode -> numOfNodes--;
					}
					break;
				}
			}
			currentBucketNode2_callee = currentBucketNode2_callee -> next;
		}
		if(i != numOfNodes -1 && flag == 0)
		{
			previousListNode = currentListNode;
			currentListNode = currentListNode -> next;
		}
	}//end for i

	if(deny == 0)
	{
		//tote mono kanei eisagogi sti lista ton sindromiti.
		if(headNode -> numOfNodes == 0)
		{
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





void indist(hashTable1 *HT1, int HT1numOfEntries, hashTable2 *HT2, int HT2numOfEntries, char *caller1, char *caller2)
{
	int i, j, k, l, m, n, subCount, flag1, flag2, key, foundYou, numOfNodes;
	char *phoneNum;
	indistList *headNode;
	subscriberNode *currentListNode;
	bucketNode1_caller *currentBucketNode1_caller;
	bucketNode2_caller *currentBucketNode2_caller, *cbn2_caller;
	bucketNode1_callee *currentBucketNode1_callee;
	bucketNode2_callee *currentBucketNode2_callee, *cbn2_callee;

	printf("[ indist1 %s %s ]\n", caller1, caller2);
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
				cbn2_caller = currentBucketNode2_caller;
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
						insertToIndistListCaller(HT1, HT1numOfEntries, HT2, HT2numOfEntries, phoneNum, headNode, cbn2_caller);
					
				}
				else if(subCount == 2)
					insertToIndistListCaller(HT1, HT1numOfEntries, HT2, HT2numOfEntries, phoneNum, headNode, cbn2_caller);
				

				free(phoneNum);
				phoneNum = NULL;
			}//end for j
			currentBucketNode1_caller = currentBucketNode1_caller -> next;
		}
	}//end for i

	//periptosi kapoios na exei mono eiserxomenes
	for(i=0; i<HT2numOfEntries; i++)
	{
		currentBucketNode1_callee = HT2[i].head2;
		while(currentBucketNode1_callee)
		{
			for(j=0; j<currentBucketNode1_callee-> nextAvailablePos; j++)
			{
				phoneNum = malloc((strlen(currentBucketNode1_callee -> b1[j].destNum)+1) * sizeof(char));
				strcpy(phoneNum, currentBucketNode1_callee -> b1[j].destNum);

				currentBucketNode2_callee = currentBucketNode1_callee -> b1[j].extraCDR;
				cbn2_callee = currentBucketNode2_callee;
				subCount = 0;
				flag1 = 0;
				flag2 = 0;
				while(currentBucketNode2_callee && subCount < 2)
				{
					for(k=0; k<currentBucketNode2_callee -> nextAvailablePos; k++)
					{
						if( (strcmp(caller1, currentBucketNode2_callee -> b2[k].origNum) == 0) && flag1 == 0 )
						{
							subCount++;
							flag1 = 1;
							if(subCount == 2)
								break;
						}
						else if( (strcmp(caller2, currentBucketNode2_callee -> b2[k].origNum) == 0) && flag2 == 0 )
						{
							subCount++;
							flag2 = 1;
							if(subCount == 2)
								break;
						}
					}
					currentBucketNode2_callee = currentBucketNode2_callee -> next;
				}
				if(subCount == 2)
					insertToIndistListCallee(HT1, HT1numOfEntries, HT2, HT2numOfEntries, phoneNum, headNode, cbn2_callee);

				free(phoneNum);
				phoneNum = NULL;

			}
			currentBucketNode1_callee = currentBucketNode1_callee -> next;
		}
	}










	currentListNode = headNode -> head;
	if(headNode -> numOfNodes == 0)
		printf("No indist found\n");
	else
	{
		for(i=0; i<headNode -> numOfNodes; i++)
		{
			printf("%s\n", currentListNode -> phoneNum);
			currentListNode = currentListNode -> next;
		}
	}	
	//apodesmeusi mnimis meta tin ektiposi
	currentListNode = headNode -> head;
	numOfNodes = headNode -> numOfNodes;
	for(i=0; i<numOfNodes; i++)
	{
		headNode -> head = currentListNode -> next;
		free(currentListNode -> phoneNum);
		currentListNode -> phoneNum = NULL;
		free(currentListNode);
		currentListNode = headNode -> head;
		headNode -> numOfNodes--;
	}
	free(headNode -> caller1);
	free(headNode -> caller2);
	headNode -> caller1 = NULL;
	headNode -> caller2 = NULL;
	free(headNode);
	headNode = NULL;
	
	printf("\n");
}