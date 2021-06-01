#include <check.h>

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

START_TEST(test_create_hashtable)
{
    HashTable *hashtable = hashtableCreate(3, sizeof(int));

    ck_assert(hashtable != NULL);
    ck_assert(hashtable->size == 3);
    ck_assert(hashtable->elementSize == 4);
    ck_assert(hashtable->data != NULL);
}
END_TEST

START_TEST(test_insert_hashtable)
{
    HashTable *hashtable = hashtableCreate(3, sizeof(int));

    int elementTest;
    elementTest = 54;
    hashtableSetElement(hashtable, 1, &elementTest);
    elementTest = 23;
    hashtableSetElement(hashtable, 6, &elementTest);
    elementTest = 64;
    hashtableSetElement(hashtable, 2, &elementTest);

    int *test;
    hashtableGetElement(hashtable, 1, (void**)&test);
    ck_assert(*test == 54);
    hashtableGetElement(hashtable, 6, (void**)&test);
    ck_assert(*test == 23);
    hashtableGetElement(hashtable, 2, (void**)&test);
    ck_assert(*test == 64);
}
END_TEST

START_TEST(test_delete_hashtable)
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

    hashtableGetElement(hashtable, 1, (void**)&test);
    ck_assert(test == NULL);
    hashtableGetElement(hashtable, 2, (void**)&test);
    ck_assert(test == NULL);
    hashtableGetElement(hashtable, 3, (void**)&test);
    ck_assert(test == NULL);
    hashtableGetElement(hashtable, 4, (void**)&test);
    ck_assert(test == NULL);
    hashtableGetElement(hashtable, 5, (void**)&test);
    ck_assert(test == NULL);
    hashtableGetElement(hashtable, 6, (void**)&test);
    ck_assert(test == NULL);
}
END_TEST

START_TEST(test_search_hashtable)
{
    HashTable *hashtable = createHashTableWithSampleData();

    int *test;
    hashtableGetElement(hashtable, 1, (void**)&test);
    ck_assert(test != NULL);
    ck_assert(*test == 54);
    hashtableGetElement(hashtable, 634, (void**)&test);
    ck_assert(test == NULL);
}
END_TEST

START_TEST(test_collision_hashtable)
{
    HashTable *hashtable = hashtableCreate(2, sizeof(4));

    int value;
    value = 4;
    hashtableSetElement(hashtable, 1, &value);
    value = 5;
    hashtableSetElement(hashtable, 3, &value);

    int *test;
    hashtableGetElement(hashtable, 1, (void**)&test);
    ck_assert(test != NULL);
    ck_assert(*test == 4);
    hashtableGetElement(hashtable, 3, (void**)&test);
    ck_assert(test != NULL);
    ck_assert(*test == 5);
}
END_TEST

Suite *hashtable_suite()
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Hashtable");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_create_hashtable);
    tcase_add_test(tc_core, test_insert_hashtable);
    tcase_add_test(tc_core, test_delete_hashtable);
    tcase_add_test(tc_core, test_search_hashtable);
    tcase_add_test(tc_core, test_collision_hashtable);

    suite_add_tcase(s, tc_core);

    return s;
}

