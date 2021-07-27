#include <criterion/criterion.h>

#include "utils/hashtable.h"

HashTable *createHashTableWithSampleData()
{
    HashTable *hashtable = hashtableCreate(3, sizeof(int));

    int elementTest;
    elementTest = 54;
    hashtableSetElement(hashtable, 1, &elementTest);
    elementTest = 23;
    hashtableSetElement(hashtable, 6, &elementTest);
    elementTest = 64;
    hashtableSetElement(hashtable, 2, &elementTest);

    return hashtable;
}

Test(hashtable, test_create_hashtable)
{
    HashTable *hashtable = hashtableCreate(3, sizeof(int));

    cr_assert(hashtable != NULL);
    cr_assert(hashtable->size == 3);
    cr_assert(hashtable->elementSize == 4);
    cr_assert(hashtable->data != NULL);
}

Test(hashtable, test_insert_hashtable)
{
    HashTable *hashtable = hashtableCreate(3, sizeof(int));

    int elementTest;
    elementTest = 54;
    hashtableSetElement(hashtable, 1, &elementTest);
    elementTest = 23;
    hashtableSetElement(hashtable, 6, &elementTest);
    elementTest = 64;
    hashtableSetElement(hashtable, 2, &elementTest);

    int *test = hashtableGetElement(hashtable, 1);
    cr_assert(*test == 54);
    test = hashtableGetElement(hashtable, 6);
    cr_assert(*test == 23);
    test = hashtableGetElement(hashtable, 2);
    cr_assert(*test == 64);
}

Test(hashtable, test_delete_hashtable)
{
    int *test;
    HashTable *hashtable = hashtableCreate(2, sizeof(int));

    int testData;
    testData = 2;
    hashtableSetElement(hashtable, 1, &testData);
    testData = 3;
    hashtableSetElement(hashtable, 2, &testData);
    testData = 4;
    hashtableSetElement(hashtable, 3, &testData);
    testData = 5;
    hashtableSetElement(hashtable, 4, &testData);
    testData = 6;
    hashtableSetElement(hashtable, 5, &testData);
    testData = 7;
    hashtableSetElement(hashtable, 6, &testData);

    hashtableDeleteElement(hashtable, 1);
    hashtableDeleteElement(hashtable, 2);
    hashtableDeleteElement(hashtable, 3);
    hashtableDeleteElement(hashtable, 4);
    hashtableDeleteElement(hashtable, 5);
    hashtableDeleteElement(hashtable, 6);

    test = hashtableGetElement(hashtable, 1); 
    cr_assert(test == NULL);
    test = hashtableGetElement(hashtable, 2);
    cr_assert(test == NULL);
    test = hashtableGetElement(hashtable, 3);
    cr_assert(test == NULL);
    test = hashtableGetElement(hashtable, 4);
    cr_assert(test == NULL);
    test = hashtableGetElement(hashtable, 5);
    cr_assert(test == NULL);
    test = hashtableGetElement(hashtable, 6);
    cr_assert(test == NULL);
}

Test(hashtable, test_search_hashtable)
{
    HashTable *hashtable = createHashTableWithSampleData();

    int *test;
    test = hashtableGetElement(hashtable, 1);
    cr_assert(test != NULL);
    cr_assert(*test == 54);
    test = hashtableGetElement(hashtable, 634);
    cr_assert(test == NULL);
}

Test(hashtable, test_collision_hashtable)
{
    HashTable *hashtable = hashtableCreate(2, 4);

    int value;
    value = 4;
    hashtableSetElement(hashtable, 1, &value);
    value = 5;
    hashtableSetElement(hashtable, 3, &value);

    int *test;
    test = hashtableGetElement(hashtable, 1);
    cr_assert(test != NULL);
    cr_assert(*test == 4);
    test = hashtableGetElement(hashtable, 3);
    cr_assert(test != NULL);
    cr_assert(*test == 5);
}

