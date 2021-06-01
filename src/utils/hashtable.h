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
    HashTableKeyValue *data;
} HashTable;

HashTable *hashtableCreate(int size, int elementSize);

void* hashtableGetElement(HashTable *ht, int key);

int hashtableSetElement(HashTable *ht, int key, void *element);

int hashtableDeleteElement(HashTable *ht, int key);

void hashtableDestroy(HashTable *ht);

#endif