/*arxikes diloseis domon kai statheron*/
#ifndef DEFINITION__H__
#define DEFINITION__H__

#define SIZE 100
#define SIZE_small 20
#define INSERT "insert"
#define DELETE "delete"
#define FIND "find"
#define CALLER "caller"
#define LOOKUP "lookup"
#define CALLEE "callee"
#define INDIST1 "indist1"
#define TOPDEST "topdest"
#define TOP "top"
#define BYE "bye"
#define PRINT "print"
#define DUMP "dump"

/******************** CALLER ********************/
typedef struct bucket2 //light pink
{
	char *cdr_uniq_id;
	char *destNum;
	char *date;
	char *time;
	int duration;
	int type;
	int tarrif;
	int fault_condition;
}bucket2;

typedef struct bucketNode2
{
	int nextAvailablePos;
	bucket2 *b2;
	struct bucketNode2 *next;
}bucketNode2;

typedef struct bucket1 //blue
{
	char *origNum;
	bucketNode2 *extraCDR;	
}bucket1;

typedef struct bucketNode1
{
	int nextAvailablePos;
	bucket1 *b1;
	struct bucketNode1 *next;
}bucketNode1;
/***********************************************/


/******************** HASHTABLE1 *******************/
typedef struct hashTable1
{
	int numOfNodes1;
	bucketNode1 *head1;
}hashTable1;
/**************************************************/








#endif