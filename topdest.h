#ifndef TOPDEST__H__
#define TOPDEST__H__

typedef struct codeNode
{
	char *code;
	int timesCalled;
	struct codeNode *next;
}codeNode;

typedef struct topdestList
{
	int numOfNodes;
	codeNode *head;
}topdestList;

void topDestCaller(hashTable1 *, int , char *);

#endif