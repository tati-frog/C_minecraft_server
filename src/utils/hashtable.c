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
    ht->data = malloc(sizeof(HashTableKeyValue) * size);
    memset(ht->data, 0, sizeof(HashTableKeyValue) * size);

    return ht;
}

HashTableKeyValue *searchKey(HashTable* ht, int key)
{
    HashTableKeyValue *keyAddress = ht->data + (sizeof(HashTableKeyValue) * hashFunction(ht, key));
    
    for(;;)
    {
        if(keyAddress->data == NULL)
        {
            return NULL;
        }

        if(keyAddress->key == key)
        {
            return keyAddress;
        }

        if(keyAddress->next == NULL) return NULL;

        keyAddress = keyAddress->next;
    }
}

int getElement(HashTable *ht, int key, void **buf)
{
    HashTableKeyValue *keyAddress = searchKey(ht, key);
    if(keyAddress == NULL){
        *buf = NULL;
        return -1;
    }

    *buf = keyAddress->data;
    return 0;
}

int setElement(HashTable *ht, int key, void *element)
{
    HashTableKeyValue *keyAddress = ht->data + (sizeof(HashTableKeyValue) * hashFunction(ht, key));

    for(;;){
        if(keyAddress->data == NULL){
            keyAddress->key = key;
            keyAddress->data = malloc(ht->elementSize);
            memcpy(keyAddress->data, element, ht->elementSize);
            return 0;
        }

        if(keyAddress->next == NULL){
            keyAddress->next = malloc(sizeof(HashTableKeyValue));

            HashTableKeyValue *actualKey = keyAddress->next;
            actualKey->key = key;
            actualKey->data = malloc(ht->elementSize);
            actualKey->next = NULL;

            memcpy(actualKey->data, element, ht->elementSize);
            return 0;
        }

        if(keyAddress->next != NULL){
            keyAddress = keyAddress->next;
        }
    }
}

int deleteElement(HashTable *ht, int key)
{
    HashTableKeyValue *head = ht->data + (sizeof(HashTableKeyValue) * hashFunction(ht, key));
    HashTableKeyValue *keyAddress = head;
    HashTableKeyValue *prevKey = NULL;

    for(;;)
    {
        if(keyAddress->data == NULL)
        {
            return -1;
        }

        if(keyAddress->key == key)
        {
            if(keyAddress == head)
            {
                free(keyAddress->data);
                if(keyAddress->next == NULL)
                {
                    memset(keyAddress, 0, sizeof(HashTableKeyValue));
                    return 0;
                }
                prevKey = keyAddress->next;
                memcpy(head, keyAddress->next, sizeof(HashTableKeyValue));
                free(prevKey);
                return 0;
            }

            if(keyAddress->next != NULL) {
                prevKey->next = keyAddress->next;
                free(keyAddress->data);
                free(keyAddress);
                return 0;
            }
            
            if(keyAddress->next == NULL){
                prevKey->next = NULL;
                free(keyAddress->data);
                free(keyAddress);
                return 0;
            }
        }

        prevKey = keyAddress;
        keyAddress = keyAddress->next;
    }
}

// TODO free memory of all the keys in the hashtable
void releaseHashtable(HashTable *ht)
{
    free(ht->data);
    free(ht);
}
