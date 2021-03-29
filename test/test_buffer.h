#include <check.h>

#include "utils/buffer.h"

START_TEST(test_create_buffer)
{
    Buffer *buffer = createBuffer();
    ck_assert(buffer != NULL);
}
END_TEST

START_TEST(test_write_data)
{
    Buffer *buffer = createBuffer();

    writeBuffer(buffer, "Hello world", 13);
    ck_assert(buffer->size == 13);
    ck_assert_str_eq(buffer->data, "Hello world");
}
END_TEST

START_TEST(test_read_data)
{
    Buffer *buffer = createBuffer();

    writeBuffer(buffer, "Hello world", 13);
    
    char str[13];
    memset(str, 0, 13);
    readBuffer(buffer, str, 13);

    ck_assert_str_eq(str, "Hello world");
    ck_assert(buffer->size == 0);

    writeBuffer(buffer, "Hello world", 13);
    
    readBuffer(buffer, str, 20);
    ck_assert_str_eq(str, "Hello world");
    ck_assert(buffer->size == 0);
}
END_TEST

Suite *buffer_suite()
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Buffer");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_create_buffer);
    tcase_add_test(tc_core, test_write_data);
    tcase_add_test(tc_core, test_read_data);

    suite_add_tcase(s, tc_core);

    return s;
}