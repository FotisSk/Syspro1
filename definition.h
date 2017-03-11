/*arxikes diloseis domon kai statheron*/
#ifndef DEFINITION__H__
#define DEFINITION__H__

#define SIZE 100
#define SIZE_small 20
#define INSERT "insert"
#define DELETE "delete"
#define FIND "find"
#define LOOKUP "lookup"
#define INDIST1 "indist1"
#define TOPDEST "topdest"
#define TOP "top"
#define BYE "bye"
#define PRINT "print"
#define DUMP "dump"

typedef struct dateStruct
{
	int day;
	int month;
	int year;
}dateStruct;

typedef struct timeStruct
{
	int hours;
	int minutes;
	int totalMinutes;
}timeStruct;

/******************** CALLER ********************/
typedef struct bucket2_caller //light pink
{
	char *cdr_uniq_id;
	char *destNum;
	char *date;
	char *time;
	int duration;
	int type;
	int tarrif;
	int fault_condition;
}bucket2_caller;

typedef struct bucketNode2_caller
{
	int nextAvailablePos;
	bucket2_caller *b2;
	struct bucketNode2_caller *next;
}bucketNode2_caller;

typedef struct bucket1_caller //blue
{
	char *origNum;
	bucketNode2_caller *extraCDR;
	int numOfNodes2;	
}bucket1_caller;

typedef struct bucketNode1_caller
{
	int nextAvailablePos;
	bucket1_caller *b1;
	struct bucketNode1_caller *next;
}bucketNode1_caller;
/***********************************************/

/******************** CALLEE ********************/
typedef struct bucket2_callee
{
	char *cdr_uniq_id;
	char *origNum;
	char *date;
	char *time;
	int duration;
	int type;
	int tarrif;
	int fault_condition;
}bucket2_callee;

typedef struct bucketNode2_callee
{
	int nextAvailablePos;
	bucket2_callee *b2;
	struct bucketNode2_callee *next;
}bucketNode2_callee;

typedef struct bucket1_callee //blue
{
	char *destNum;
	bucketNode2_callee *extraCDR;
	int numOfNodes2;	
}bucket1_callee;

typedef struct bucketNode1_callee
{
	int nextAvailablePos;
	bucket1_callee *b1;
	struct bucketNode1_callee *next;
}bucketNode1_callee;
/***********************************************/


/******************** HASHTABLE1 *******************/
typedef struct hashTable1
{
	int numOfNodes1;
	bucketNode1_caller *head1;
}hashTable1;
/**************************************************/

/******************** HASHTABLE2 *******************/
typedef struct hashTable2
{
	int numOfNodes2;
	bucketNode1_callee *head2;
}hashTable2;
/**************************************************/







#endif