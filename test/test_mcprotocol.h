#include "net/mcprotocol.h"

#include <check.h>

START_TEST(test_read_varint)
{
    Buffer *inputData = createBuffer();
    writeBuffer(inputData, "\xaf\x2a", 2);

    mc_int integer;
    int varintByteSize = mcVarintRead(inputData, &integer);

    ck_assert(integer == 5423);
    ck_assert(varintByteSize == 2);
    ck_assert(inputData->size == 0);

    writeBuffer(inputData, "\xaf", 1);

    varintByteSize = mcVarintRead(inputData, &integer);
    ck_assert(integer == 0);
    ck_assert(varintByteSize == -1);
    ck_assert(inputData->size == 0);

    releaseBuffer(inputData);
}
END_TEST

START_TEST(test_write_varint)
{
    int outputBuffer = 0;

    int varintByteSize = mcVarintWrite(5423, (char *)&outputBuffer);

    ck_assert(varintByteSize == 2);
    ck_assert(outputBuffer == 0x2aaf);
}
END_TEST


START_TEST(test_read_string)
{
    Buffer *inputBuffer = createBuffer();

    writeBuffer(inputBuffer, "\x1bHello world this is a test.", 28);

    mc_string string;
    int stringLength = mcStringRead(inputBuffer, &string);

    ck_assert_str_eq(string, "Hello world this is a test.");
    ck_assert(stringLength == 28);
    ck_assert(inputBuffer->size == 0);

    releaseBuffer(inputBuffer);
}
END_TEST

START_TEST(test_write_string)
{
    Buffer *outputBuffer = createBuffer();

    mcStringWrite(outputBuffer, "This is a test.");

    ck_assert(0 == memcmp(outputBuffer->data, "\xfThis is a test.", 16));
    ck_assert(outputBuffer->size == 16);
}
END_TEST


START_TEST(test_read_packet)
{
    Buffer *inputBuffer = createBuffer();

    writeBuffer(inputBuffer, "\x04\x02sus", 5);

    MCPacket packet;
    mcPacketCreate(&packet);

    mcPacketRead(inputBuffer, &packet);

    ck_assert(packet.length == 4);
    ck_assert(packet.id == 2);
    ck_assert(0 == memcmp(packet.data->data, "sus", 3));
}
END_TEST

START_TEST(test_write_packet)
{
    Buffer *outputBuffer = createBuffer();

    MCPacket packet;
    mcPacketCreate(&packet);

    packet.id = 1;
    writeBuffer(packet.data, "sample data", 11);

    mcPacketWrite(outputBuffer, &packet);

    ck_assert(outputBuffer->size == 13);
    ck_assert(0 == memcmp(outputBuffer->data, "\x0c\x01sample data", 13));
}
END_TEST


START_TEST(test_create_player)
{

}
END_TEST

Suite *mcprotocol_suite()
{
    Suite *suite = suite_create("Mc protocol");

    TCase *dataTypesCase = tcase_create("Datatypes");
    tcase_add_test(dataTypesCase, test_write_varint);
    tcase_add_test(dataTypesCase, test_read_varint);
    tcase_add_test(dataTypesCase, test_write_string);
    tcase_add_test(dataTypesCase, test_read_string);

    TCase *packetsCase = tcase_create("Packets");
    tcase_add_test(packetsCase, test_write_packet);
    tcase_add_test(packetsCase, test_read_packet);

    TCase *miscCase = tcase_create("misc");
    tcase_add_test(miscCase, test_create_player);

    suite_add_tcase(suite, dataTypesCase);
    suite_add_tcase(suite, packetsCase);
    suite_add_tcase(suite, miscCase);
    return suite;
}