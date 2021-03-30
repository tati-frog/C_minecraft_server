#include <check.h>

#include "nbt/nbt.h"

START_TEST(test_create_end_tag)
{
    NBT_Tag *endTag = nbtEndTagCreate();

    ck_assert(endTag->tagType == 0);
    ck_assert(endTag->payload == 0);
    ck_assert(endTag->name.length == 0);
    ck_assert(endTag->name.payload == 0);
}
END_TEST

Suite *nbt_suite()
{
    Suite *suite = suite_create("NBT");

    TCase *tagsCase = tcase_create("Tags");
    tcase_add_test(tagsCase, test_create_end_tag);

    suite_add_tcase(suite, tagsCase);

    return suite;
}