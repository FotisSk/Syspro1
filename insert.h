#ifndef INSERT__H__
#define INSERT__H__

int hashFunction(char *origNum, int HT1numOfEntries);
int insert(hashTable1 *HT1, int HT1numOfEntries, int bucket1_maxEntries, int bucket2_maxEntries, char *cdr_uniq_id, char *origNum, char *destNum, char *date, char *time, int duration, int type, int tarrif, int fault_condition);

#endif