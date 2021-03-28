#ifndef _HASHTABLE
#define _HASHTABLE

typedef struct s_HashTableElement{
    int key;
    void *data;
    struct s_HashTableElement *next;
} HashTableKeyValue;

typedef struct {
    int elementSize;
    int size;
    void *data;
} HashTable;

HashTable *createHashtable(int size, int elementSize);

int getElement(HashTable *ht, int key, void **buf);

int setElement(HashTable *ht, int key, void *element);

int deleteElement(HashTable *ht, int key);

int releaseHashtable(HashTable *ht);

#endif