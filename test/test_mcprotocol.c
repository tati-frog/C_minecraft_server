#include "net/mcprotocol.h"

#include <criterion/criterion.h>

Test(mcprotocol, test_read_varint)
{
    Buffer *inputData = createBuffer();
    writeBuffer(inputData, "\xaf\x2a", 2);

    mc_int integer;
    int varintByteSize = mcVarintRead(inputData, &integer);

    cr_assert(integer == 5423);
    cr_assert(varintByteSize == 2);
    cr_assert(inputData->size == 0);

    writeBuffer(inputData, "\xaf", 1);

    varintByteSize = mcVarintRead(inputData, &integer);
    cr_assert(integer == 0);
    cr_assert(varintByteSize == -1);
    cr_assert(inputData->size == 0);

    releaseBuffer(inputData);
}


Test(mcprotocol, test_write_varint)
{
    int outputBuffer = 0;

    int varintByteSize = mcVarintWrite(5423, (char *)&outputBuffer);

    cr_assert(varintByteSize == 2);
    cr_assert(outputBuffer == 0x2aaf);
}



Test(mcprotocol, test_read_string)
{
    Buffer *inputBuffer = createBuffer();

    writeBuffer(inputBuffer, "\x1bHello world this is a test.", 28);

    mc_string string;
    int stringLength = mcStringRead(inputBuffer, &string);

    cr_assert_str_eq(string, "Hello world this is a test.");
    cr_assert(stringLength == 28);
    cr_assert(inputBuffer->size == 0);

    releaseBuffer(inputBuffer);
}


Test(mcprotocol, test_write_string)
{
    Buffer *outputBuffer = createBuffer();

    mcStringWrite(outputBuffer, "This is a test.");

    cr_assert(0 == memcmp(outputBuffer->data, "\xfThis is a test.", 16));
    cr_assert(outputBuffer->size == 16);
}



Test(mcprotocol, test_read_packet)
{
    Buffer *inputBuffer = createBuffer();

    writeBuffer(inputBuffer, "\x04\x02sus", 5);

    MCPacket packet;
    mcPacketCreate(&packet);

    mcPacketRead(inputBuffer, &packet);

    cr_assert(packet.length == 4);
    cr_assert(packet.id == 2);
    cr_assert(0 == memcmp(packet.data->data, "sus", 3));
}


Test(mcprotocol, test_write_packet)
{
    Buffer *outputBuffer = createBuffer();

    MCPacket packet;
    mcPacketCreate(&packet);

    packet.id = 1;
    writeBuffer(packet.data, "sample data", 11);

    mcPacketWrite(outputBuffer, &packet);

    cr_assert(outputBuffer->size == 13);
    cr_assert(0 == memcmp(outputBuffer->data, "\x0c\x01sample data", 13));
}



Test(mcprotocol, test_create_player)
{

}

