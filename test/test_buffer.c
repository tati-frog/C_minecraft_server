#include <criterion/criterion.h>

#include "utils/buffer.h"

Test(buffer, test_create_buffer)
{
    Buffer *buffer = createBuffer();
    cr_assert(buffer != NULL);
}

Test(buffer, test_write_data)
{
    Buffer *buffer = createBuffer();

    writeBuffer(buffer, "Hello world", 13);
    cr_assert(buffer->size == 13);
    cr_assert_str_eq(buffer->data, "Hello world");
}

Test(buffer, test_read_data)
{
    Buffer *buffer = createBuffer();

    writeBuffer(buffer, "Hello world", 13);
    
    char str[13];
    memset(str, 0, 13);
    readBuffer(buffer, str, 13);

    cr_assert_str_eq(str, "Hello world");
    cr_assert(buffer->size == 0);

    writeBuffer(buffer, "Hello world", 13);
    
    readBuffer(buffer, str, 20);
    cr_assert_str_eq(str, "Hello world");
    cr_assert(buffer->size == 0);
}

