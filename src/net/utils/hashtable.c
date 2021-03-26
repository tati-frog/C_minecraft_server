#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

int hashFunction(HashTable *ht, int n)
{
    return n % ht->size;
}

HashTable *createHashtable(int size, int elementSize)
{
    HashTable *ht = malloc(sizeof(HashTable));
    memset(ht, 0, sizeof(HashTable));

    ht->size = size;
    ht->elementSize = elementSize;
    ht->data = malloc(size * elementSize);
    memset(ht->data, 0, size * elementSize);

    return ht;
}

int getElement(HashTable *ht, int key, void **buf)
{
    void *elementAddress = ht->data + (ht->elementSize * hashFunction(ht, key));
    *buf = elementAddress;
}

int setElement(HashTable *ht, int key, void *element)
{
    void *elementAddress = ht->data + (ht->elementSize * hashFunction(ht, key));
    memcpy(elementAddress, element, ht->elementSize);
}

int deleteElement(HashTable *ht, int key)
{
    void *elementAddress = ht->data + (ht->elementSize * hashFunction(ht, key));
    memset(elementAddress, 0, ht->elementSize);
}

int releaseHashtable(HashTable *ht)
{
    free(ht->data);
    free(ht);
}
