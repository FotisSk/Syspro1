#ifndef INSERT__H__
#define INSERT__H__

int hashFunction(char *, int );
dateStruct dateToIntegers(char *);
timeStruct timeToIntegers(char *);
int insertCaller(heap *, hashTable1 *, int , int , int , char *, char *, char *, char *, char *, int ,int ,int , int , chargeListNode *);
int insertCallee(hashTable2 *, int , int , int , char *, char *, char *, char *, char *, int ,int ,int , int );
int insertNode(heap *, bucketNode1_caller *, int , hashTable1 *, int , char *, int , int , int , int , chargeListNode *);
int heapify(heap *, heapNode *, bucketNode1_caller *, int );
int updateNode(heap *, bucketNode1_caller *, int , int , int , int , int , chargeListNode *);

#endif