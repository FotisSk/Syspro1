#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "definition.h"
#include "insert.h"
#include "delete.h"
#include "find.h"
#include "lookup.h"
#include "indist1.h"
#include "topdest.h"
#include "top.h"

void topKPercent(heap *hp, double percentage, hashTable1 *HT1, int HT1numOfEntries)
{
	int i,j, flag, foundAtLeastOneChild;
	double companyRevenue, revenue_topK, remainingRevenue_topK, subscriberRevenuePercentage, maxAmount;
	bucketNode1_caller *currentBucketNode1;
	topK_listNode *topK_listHead, *currentTopKListNode, *previousTopKListNode;
	heapNode *maxChild;

	printf("[ top %.0f ]\n", percentage);
	if(!hp->head)
	{
		printf("No calls found (Heap is empty)\n\n");
		return;
	}
	//vres to sinoliko eisodima tis eitaireias
	companyRevenue = 0.0;
	for(i=0; i<HT1numOfEntries; i++)
	{
		currentBucketNode1 = HT1[i].head1;
		while(currentBucketNode1)
		{
			for(j=0; j<currentBucketNode1 -> nextAvailablePos; j++)
				companyRevenue += currentBucketNode1 -> b1[j].heapPtr -> amount;

			currentBucketNode1 = currentBucketNode1 -> next;
		}
	}
	//printf("Revenue: %f\n", companyRevenue);
	revenue_topK = (companyRevenue * (percentage/100))+0.00001;
	//printf("Top %.0f%% company revenue: %f\n", percentage, revenue_topK);

	remainingRevenue_topK = revenue_topK;
	//an to proto stoixeio tou heap exei megalitero amount apo to zitoumeno revenue_topK tote den kanoume tipota
	if(hp -> head-> amount > revenue_topK)
	{
		printf("No calls found (heap's head amount > Top %.0f%% company revenue)\n\n", percentage);
		return;
	}
	else if(hp -> head -> amount <= revenue_topK && hp -> head -> amount  != 0)
	{
		remainingRevenue_topK -= hp -> head -> amount;
		//arxi voithitikis listas, vazoume sigoura to proto stoixeio tou heap.
		topK_listHead = malloc(sizeof(topK_listNode));
		topK_listHead -> subscriber = malloc((strlen(hp -> head -> subscriber)+1) * sizeof(char));
		strcpy(topK_listHead -> subscriber, hp -> head -> subscriber);
		topK_listHead -> amount = hp -> head -> amount;
		topK_listHead -> heapPtr = hp -> head -> bucketNodePtr -> b1[hp -> head -> bucketPos].heapPtr;
		topK_listHead -> heapPtr -> inTopKList = 1;
		topK_listHead -> next = NULL;
	}
	else if(hp -> head -> amount <= revenue_topK && hp -> head -> amount  == 0)
	{
		printf("No calls found (heap's head amount = 0)\n\n");
		return;
	}

	flag = 0;
	while(flag == 0)	//oso ta max paidia pou vriskeis exoun amount mikrotero apo to remainingRevenue_topK
	{
		currentTopKListNode = topK_listHead;
		previousTopKListNode = NULL;
		maxAmount = 0; 
		maxChild = NULL;
		foundAtLeastOneChild = 0;
		//diatrexei ti lista mas kai fernei piso to megalitero paidi.
		while(currentTopKListNode)
		{
			if(currentTopKListNode -> heapPtr -> leftChild)
			{
				if(currentTopKListNode -> heapPtr -> leftChild -> inTopKList == 0)
				{	
					if(currentTopKListNode -> heapPtr -> leftChild -> amount > maxAmount)
					{
						foundAtLeastOneChild = 1;
						maxAmount = currentTopKListNode -> heapPtr -> leftChild -> amount;
						maxChild = currentTopKListNode -> heapPtr -> leftChild;
					}
				}
			}
			if(currentTopKListNode -> heapPtr -> rightChild)
			{
				if(currentTopKListNode -> heapPtr -> rightChild -> inTopKList == 0)
				{
					if(currentTopKListNode -> heapPtr -> rightChild -> amount > maxAmount)
					{
						foundAtLeastOneChild = 1;
						maxAmount = currentTopKListNode -> heapPtr -> rightChild -> amount;
						maxChild = currentTopKListNode -> heapPtr -> rightChild;
					}
				}
			}
			previousTopKListNode = currentTopKListNode;
			currentTopKListNode = currentTopKListNode -> next;
		}

		if(foundAtLeastOneChild == 1)
		{
			//molis mpeis edo exeis to paidi me to max amount
			if(maxChild -> amount <= remainingRevenue_topK)
			{
				//tote valto sti lista, mas kanei
				previousTopKListNode -> next = malloc(sizeof(topK_listNode));
				currentTopKListNode = previousTopKListNode -> next;
				currentTopKListNode -> subscriber = malloc((strlen(maxChild -> subscriber)+1) * sizeof(char));
				strcpy(currentTopKListNode -> subscriber, maxChild -> subscriber);
				currentTopKListNode -> amount = maxAmount; //i maxChild -> amount
				currentTopKListNode -> heapPtr = maxChild;
				currentTopKListNode -> next = NULL;

				maxChild -> inTopKList = 1;

				remainingRevenue_topK -= maxAmount;	//i maxChild -> amount
			}
			else
				flag = 1;
		}
		else
			flag = 1;
		
	}

	//print ti lista
	currentTopKListNode = topK_listHead;
	while(currentTopKListNode)
	{
		subscriberRevenuePercentage = (currentTopKListNode -> amount * 100) / companyRevenue;
		printf("%s %.6f%%\n", currentTopKListNode -> subscriber, subscriberRevenuePercentage);

		currentTopKListNode = currentTopKListNode -> next;
	}

	//clean memory
	currentTopKListNode = topK_listHead;
	while(currentTopKListNode)
	{
		topK_listHead = currentTopKListNode -> next;
		free(currentTopKListNode -> subscriber);
		currentTopKListNode -> subscriber = NULL;
		currentTopKListNode -> heapPtr -> inTopKList = 0;
		free(currentTopKListNode);
		currentTopKListNode = topK_listHead;
	}
	printf("\n");
	return;

}
