#ifndef INSERT__H__
#define INSERT__H__

int hashFunction(char *, int );
dateStruct dateToIntegers(char *);
timeStruct timeToIntegers(char *);
int insertCaller(hashTable1 *, int , int , int , char *, char *, char *, char *, char *, int ,int ,int , int );
int insertCallee(hashTable2 *, int , int , int , char *, char *, char *, char *, char *, int ,int ,int , int );

#endif