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

void topDestCaller(hashTable1 *HT1, int HT1numOfEntries, char *caller)
{
	int key, numOfNodes, flag, flag2, i, j, k, numOfListNodes, maxCalls;
	char *split, *maxCode; 
	char phone[15];
	bucketNode1_caller *head, *currentBucketNode1;
	bucketNode2_caller *currentBucketNode2, *initialBucketNode2;
	codeNode *currentListNode, *previousListNode, *listNode, *maxListNode;
	topdestList *headNode;

	headNode = malloc(sizeof(topdestList));
	headNode -> numOfNodes = 0;
	headNode -> head = NULL;

	key = hashFunction(caller, HT1numOfEntries);

	numOfNodes = HT1[key].numOfNodes1;
	head = HT1[key].head1;
	currentBucketNode1 = head;
	flag = 0;

	while(currentBucketNode1 && flag == 0)
	{
		for(i=0; i<currentBucketNode1 -> nextAvailablePos; i++)
		{
			if(strcmp(caller, currentBucketNode1 -> b1[i].origNum) == 0)
			{
				flag = 1;
				initialBucketNode2 = currentBucketNode1 -> b1[i].extraCDR;
				break;
			}
		}
		if(flag == 0)
			currentBucketNode1 = currentBucketNode1 -> next;
	}

	if(flag == 0)
	{
		printf("Caller '%s' not found.\n", caller);
		return;
	}
	currentBucketNode2 = initialBucketNode2;
	while(currentBucketNode2)
	{
		for(j=0; j<currentBucketNode2 -> nextAvailablePos; j++)
		{
			strcpy(phone, currentBucketNode2 -> b2[j].destNum);
			split = strtok(phone, "-\r\n");

			previousListNode = NULL;
			currentListNode = headNode -> head;
			flag2 = 0;
			while(currentListNode && flag2 == 0)
			{
				if(strcmp(currentListNode -> code, split) == 0)
				{
					flag2 = 1;
					currentListNode -> timesCalled++;		
				}
				else
				{
					previousListNode = currentListNode;
					currentListNode = currentListNode -> next;
				}		
			}
			if(flag2 == 0)	//o kodikos den iparxei sti lista, prosthese komvo
			{
				listNode = malloc(sizeof(codeNode));
				listNode -> code = malloc((strlen(split)+1) * sizeof(char));
				strcpy(listNode -> code, split);
				listNode -> timesCalled = 1;
				listNode -> next = headNode -> head;

				headNode -> head = listNode;
				headNode -> numOfNodes++;
			}
		}
		currentBucketNode2 = currentBucketNode2 -> next;		
	}
	//ektiposi olis tis listas (gia mena)
	/*
	currentListNode = headNode -> head;
	printf("####### %s #######\n", caller);
	for(k=0; k<headNode -> numOfNodes; k++)
	{
		printf("{ Code: %s | Times Called: %d}\n", currentListNode -> code, currentListNode -> timesCalled);
		currentListNode = currentListNode -> next;
	}
	printf("##############################\n\n");
	*/
	maxListNode = NULL;
	maxCalls = 0;
	currentListNode = headNode -> head;
	printf("*** TOPDEST %s ***\n", caller);
	for(k=0; k<headNode -> numOfNodes; k++)
	{
		if(currentListNode -> timesCalled > maxCalls)
		{
			maxCalls = currentListNode -> timesCalled;
			maxListNode = currentListNode;
		}
		currentListNode = currentListNode -> next;
	}
	printf("| Code: %s | Times Called: %d|\n", maxListNode -> code, maxCalls);

	currentListNode = headNode -> head;
	for(k=0; k<headNode -> numOfNodes; k++)
	{
		if(currentListNode -> timesCalled == maxCalls && currentListNode != maxListNode)
			printf("| Code: %s | Times Called: %d|\n", currentListNode -> code, currentListNode -> timesCalled);

		currentListNode = currentListNode -> next;
	}
	printf("******************************\n\n");

	//apodesmeusi listas
	numOfListNodes = headNode -> numOfNodes;
	currentListNode = headNode -> head;
	for(k=0; k<numOfListNodes; k++)
	{
		headNode -> head = currentListNode -> next;
		free(currentListNode -> code);
		currentListNode -> code = NULL;
		free(currentListNode);
		currentListNode = headNode -> head;
		headNode -> numOfNodes--;
	}
	free(headNode);
	headNode = NULL;
	return;
}

