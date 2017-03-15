#ifndef INDIST1__H__
#define INDIST1__H__

typedef struct subscriberNode
{
	char *phoneNum;
	struct subscriberNode *next;
}subscriberNode;

typedef struct indistList
{
	char *caller1;
	char *caller2;

	int numOfNodes;
	subscriberNode *head;
}indistList;

int insertToIndistList(hashTable1 *, int, hashTable2 *, int, char *, indistList *, bucketNode2_caller *);
void indist(hashTable1 *, int , hashTable2 *, int , char *, char *);

#endif