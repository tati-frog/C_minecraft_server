#include "test_mcprotocol.h"
#include "test_buffer.h"
#include "test_hashtable.h"
#include <check.h>

void runRunner(SRunner *sr)
{
    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_VERBOSE);
    srunner_free(sr);
}

int main()
{
    Suite *hashtable;
    SRunner *hashtableRunner;

    Suite *buffer;
    SRunner *bufferRunner;

    Suite *mcprotocol;
    SRunner *mcprotocolRunner;

    hashtable = hashtable_suite();
    hashtableRunner = srunner_create(hashtable);

    runRunner(hashtableRunner);

    buffer = buffer_suite();
    bufferRunner = srunner_create(buffer);

    runRunner(bufferRunner);

    mcprotocol = mcprotocol_suite();
    mcprotocolRunner = srunner_create(mcprotocol);

    runRunner(mcprotocolRunner);
    return 0;
}