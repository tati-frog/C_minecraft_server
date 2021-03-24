#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>

#include "mcprotocol.h"
#include "./utils/buffer.h"

// Read a varint from a file descriptor.
int readVarint(Buffer *buffer, mc_int *buf)
{
    int numRead = 0;
    int result = 0;
    char read;
    do
    {
        readBuffer(buffer, &read, 1);
        int value = (read & 0b01111111);
        result |= (value << (7 * numRead));

        numRead++;
        if (numRead > 5)
        {
        }
    } while ((read & 0b10000000) != 0);

    *buf = result;

    return numRead;
}
// Serialize a value into a varint and save the byte array into buf.
int serializeIntoVarint(mc_int value, char *buf)
{
    int i = 0;

    do
    {
        char temp = (char)(value & 0b01111111);

        value = value >> 7;
        if (value != 0)
        {
            temp |= 0b10000000;
        }

        buf[i] = temp;
        i++;
    } while (value != 0);

    return i;
}

// Read a string from a file descriptor. The string is saved on the heap.
int readString(Buffer *buffer, mc_string *buf)
{
    int readedBytes;
    mc_int lenght;
    readedBytes = readVarint(buffer, &lenght);

    mc_string string;
    string.size = lenght;
    string.data = malloc(lenght);

    readedBytes += readBuffer(buffer, string.data, string.size);

    return readedBytes;
}

// Read a packet from a file descriptor. The data is allocated in the heap.
int readPacket(Buffer *buffer, MCPacket *buf)
{
    mc_int packetSize;
    readVarint(buffer, &packetSize);

    MCPacket packet;
    packet.lenght = packetSize;
    int packetIdSize = readVarint(buffer, &packet.id);

    packet.dataSize = packet.lenght - packetIdSize;
    packet.data = malloc(packet.dataSize);
    int readed = readBuffer(buffer, packet.data, packet.dataSize);

    *buf = packet;

    return readed;
}
// Write a packet into a buffer.
int writePacket(Buffer *buffer, MCPacket *packet)
{
    char varintPacketSize[4];
    int varintPacketSizeLenght = serializeIntoVarint(packet->lenght, varintPacketSize);

    char varintPacketid[4];
    int varintPacketidLenght = serializeIntoVarint(packet->id, varintPacketid);

    writeBuffer(buffer, varintPacketSize, varintPacketSizeLenght);
    writeBuffer(buffer, varintPacketid, varintPacketidLenght);
    writeBuffer(buffer, packet->data, packet->dataSize);
}