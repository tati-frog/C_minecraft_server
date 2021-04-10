#include "test_nbt.h"
#include "test_mcprotocol.h"
#include "test_buffer.h"
#include "test_hashtable.h"
#include <check.h>

#define DEBUG 0

void runRunner(SRunner *sr)
{
    #if DEBUG == 1
    srunner_set_fork_status(sr, CK_NOFORK);
    #endif
    srunner_run_all(sr, CK_VERBOSE);
    srunner_free(sr);
}

int main()
{
    Suite *hashtable = hashtable_suite();
    SRunner *hashtableRunner = srunner_create(hashtable);

    runRunner(hashtableRunner);

    Suite *buffer = buffer_suite();
    SRunner *bufferRunner = srunner_create(buffer);

    runRunner(bufferRunner);

    Suite *mcprotocol = mcprotocol_suite();
    SRunner *mcprotocolRunner = srunner_create(mcprotocol);

    runRunner(mcprotocolRunner);

    Suite *nbt = nbt_suite();
    SRunner *nbtRunner = srunner_create(nbt);

    runRunner(nbtRunner);
    return 0;
}
