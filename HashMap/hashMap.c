#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hashMap.h"

struct hashLink {
   KeyType key; /*the key is what you use to look up a hashLink*/
   ValueType value; /*the value stored with the hashLink, a pointer to int in the case of concordance*/
   struct hashLink * next; /*notice how these are like linked list nodes*/
};
typedef struct hashLink hashLink;

struct hashMap {
    hashLink ** table; /*array of pointers to hashLinks*/
    int tableSize; /*number of buckets in the table*/
    int count; /*number of hashLinks in the table*/
};
typedef struct hashMap hashMap;

/*the first hashing function you can use*/
int stringHash1(char * str)
{
	int i;
	int r = 0;
	for (i = 0; str[i] != '\0'; i++)
		r += str[i];
	return r;
}

/*the second hashing function you can use*/
int stringHash2(char * str)
{
	int i;
	int r = 0;
	for (i = 0; str[i] != '\0'; i++)
		r += (i+1) * str[i]; /*the difference between stringHash1 and stringHash2 is on this line*/
	return r;
}

/* initialize the supplied hashMap struct*/
void _initMap (struct hashMap * ht, int tableSize)
{
	int index;
	if(ht == NULL)
		return;
	ht->table = (hashLink**)malloc(sizeof(hashLink*) * tableSize);
	ht->tableSize = tableSize;
	ht->count = 0;
	for(index = 0; index < tableSize; index++)
		ht->table[index] = NULL;
}

/* allocate memory and initialize a hash map*/
hashMap *createMap(int tableSize) {
	assert(tableSize > 0);
	hashMap *ht;
	ht = malloc(sizeof(hashMap));
	assert(ht != 0);
	_initMap(ht, tableSize);
	return ht;
}

/*
 Free all memory used by the buckets.
 */
void _freeLinks (struct hashLink **table, int tableSize)
{
    int i;
    struct hashLink *cur, *temp;
    for (i = 0; i < tableSize; i++) {
        if (table[i] != NULL) {
            cur = table[i];
            while (cur != NULL) {
                temp = cur;
                cur = cur->next;
                free(temp->key);
                free(temp->value);
                free(temp);
            }
        }
    }
}

/* Deallocate buckets, table, and hashTable structure itself.*/
void deleteMap(hashMap *ht) {


	assert(ht!= 0);
	/* Free all memory used by the buckets */
	_freeLinks(ht->table, ht->tableSize);
        /* Free the array of buckets */
        free(ht->table);
	/* free the hashMap struct */
	free(ht);
}

/*
Resizes the hash table to be the size newTableSize
*/
void _setTableSize(struct hashMap * ht, int newTableSize)
{
    printf("========Resizing========\n");
    struct hashLink **oldTable;
    oldTable = ht->table;
    int oldSize = ht->tableSize;
    struct hashLink *cur, *temp;
    int i;
    _initMap(ht, newTableSize);
    for (i = 0; i < oldSize; i++) {
        cur = oldTable[i];
        while (cur != NULL) {
            insertMap(ht, cur->key, cur->value);
            temp = cur;
            cur = cur->next;
            free(temp);
        }
    }
    free(oldTable);
}

/*
 insert the following values into a hashLink, you must create this hashLink but
 only after you confirm that this key does not already exist in the table. For example, you
 cannot have two hashLinks for the word "taco".

 if a hashLink already exists in the table for the key provided you should
 replace the value for that key.  As the developer, you DO NOT FREE UP THE MEMORY FOR THE VALUE.
 We have to leave that up to the user of the hashMap to take care of since they may or may not
 have allocated the space with malloc.


 Also, you must monitor the load factor and resize when the load factor is greater than
 or equal LOAD_FACTOR_THRESHOLD (defined in hashMap.h).
 */
void insertMap (struct hashMap * ht, KeyType k, ValueType v)
{
    int hashIndex = stringHash2(k) % ht->tableSize;
    if (hashIndex < 0) hashIndex += ht->tableSize;
    if (containsKey(ht, k) == 0) {
        struct hashLink* newLink = (hashLink*)malloc(sizeof(struct hashLink));
        assert(newLink != NULL);
        newLink->key = k;
        newLink->value = v;
        newLink->next = ht->table[hashIndex];
        ht->table[hashIndex] = newLink;
        ht->count++;
    } else {
        struct hashLink* cur = ht->table[hashIndex];
        while (cur != NULL) {
            if (strcmp(cur->key,k) == 0) {
                cur->value = v;
                break;
            }
            cur = cur->next;
        }
    }
    float loadFactor = (float)ht->count/(float)ht->tableSize;
    if (loadFactor >= LOAD_FACTOR_THRESHOLD)
        _setTableSize(ht, ht->tableSize * 2);
}

/*
 this returns the value (which is void*) stored in a hashLink specified by the key k.

 if the user supplies the key "taco" you should find taco in the hashTable, then
 return the value member of the hashLink that represents taco.

 if the supplied key is not in the hashtable return NULL.
 */
ValueType atMap (struct hashMap * ht, KeyType k)
{
    if (containsKey(ht, k)) {
        int hashIndex = stringHash2(k) % ht->tableSize;
        if (hashIndex < 0) hashIndex += ht->tableSize;
        struct hashLink* cur = ht->table[hashIndex];
        while (cur != NULL) {
            if (strcmp(cur->key,k) == 0)
                return cur->value;
            cur = cur->next;
        }
    } else return 0;
    return 0;
}

/*
 a simple yes/no if the key is in the hashtable.
 0 is no, all other values are yes.
 */
int containsKey (struct hashMap * ht, KeyType k)
{
    int hashIndex = stringHash2(k) % ht->tableSize;
    if (hashIndex < 0) hashIndex += ht->tableSize;
    struct hashLink* cur = ht->table[hashIndex];
    while (cur != NULL) {
        if (strcmp(cur->key,k) == 0)
            return 1;
        cur = cur->next;
    }
    return 0;
}

/*
 find the hashlink for the supplied key and remove it, also freeing the memory
 for that hashlink. it is not an error to be unable to find the hashlink, if it
 cannot be found do nothing (or print a message) but do not use an assert which
 will end your program.
 */
void removeKey (struct hashMap * ht, KeyType k)
{
    if (containsKey(ht, k) == 0)
        printf("Error: Unable to find hashLink to be removed.\n");
    else {
        int hashIndex = stringHash2(k) % ht->tableSize;
        if (hashIndex < 0) hashIndex += ht->tableSize;
        struct hashLink* cur = ht->table[hashIndex];
        struct hashLink* temp = ht->table[hashIndex];
        
        while (cur != NULL) {
            if (strcmp(cur->key,k) == 0) {
                if (cur == ht->table[hashIndex])
                    ht->table[hashIndex] = cur->next;
                else
                    temp->next = cur->next;
                free(cur->value);
                free(cur->key);
                free(cur);
                ht->count--;
                break;
            } else {
                temp = cur;
                cur = cur->next;
            }
        }
    }
}

/*
 returns the number of hashLinks in the table
 */
int size (struct hashMap *ht)
{
    assert(ht != NULL);
	return ht->count;
}

/*
 returns the number of buckets in the table
 */
int capacity(struct hashMap *ht)
{
    assert(ht != NULL);
	return ht->tableSize;
}

/*
 returns the number of empty buckets in the table, these are buckets which have
 no hashlinks hanging off of them.
 */
int emptyBuckets(struct hashMap *ht)
{
    assert(ht != NULL);
	int count = 0;
    int i;
    for (i = 0; i < ht->tableSize; i++) {
        if (ht->table[i] == NULL)
            count++;
    }
	return count;
}

/*
 returns the ratio of: (number of hashlinks) / (number of buckets)

 this value can range anywhere from zero (an empty table) to more then 1, which
 would mean that there are more hashlinks then buckets (but remember hashlinks
 are like linked list nodes so they can hang from each other)
 */
float tableLoad(struct hashMap *ht)
{
    assert(ht != NULL);
    return (float)ht->count/(float)ht->tableSize;
}

/* print the hashMap */
 void printMap (struct hashMap * ht, keyPrinter kp, valPrinter vp)
{
	int i;
	struct hashLink *temp;
	for(i = 0;i < capacity(ht); i++){
		temp = ht->table[i];
		if(temp != 0) {
			printf("\nBucket Index %d -> ", i);
		}
		while(temp != 0){
                        printf("Key:");
                        (*kp)(temp->key);
                        printf("|Value:");
                        (*vp)(temp->value);
			printf(" -> ");
			temp=temp->next;
		}
	}
}

/* print the keys/values ..in linear form, no buckets */
 void printKeyValues (struct hashMap * ht, keyPrinter kp, valPrinter vp)
{
	int i;
	struct hashLink *temp;
	for(i = 0;i < capacity(ht); i++){
		temp = ht->table[i];
		while(temp != 0){
                        (*kp)(temp->key);
			printf(":");
                        (*vp)(temp->value);
			printf("\n");
			temp=temp->next;
		}
	}
}
