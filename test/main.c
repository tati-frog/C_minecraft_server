#include "test_hashtable.h"
#include <check.h>

int main()
{
    Suite *hashtable;
    SRunner *sr;

    hashtable = hashtable_suite();
    sr = srunner_create(hashtable);
    srunner_set_fork_status(sr, CK_NOFORK);

    srunner_run_all(sr, CK_VERBOSE);

    srunner_free(sr);
    return 0;
}