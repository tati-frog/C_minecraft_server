#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>

#include "mcprotocol.h"

// Read a varint from a file descriptor.
int readVarint(int fd, mc_int *buf)
{
    int numRead = 0;
    int result = 0;
    char read;
    do
    {
        recv(fd, &read, 1, 0);
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
    } while (value != 0);

    return i + 1;
}

// Read a string from a file descriptor. The string is saved on the heap.
int readString(int fd, mc_string *buf)
{
}

// Read a packet from a file descriptor. The data is allocated in the heap.
int readPacket(int fd, MCPacket *buf)
{
    mc_int packetSize;
    readVarint(fd, &packetSize);

    MCPacket packet;
    packet.lenght = packetSize;
    int packetIdSize = readVarint(fd, &packet.id);

    packet.dataSize = packet.lenght - packetIdSize;
    packet.data = malloc(packet.dataSize);
    int readed = recv(fd, packet.data, packet.dataSize, 0);

    *buf = packet;

    return readed;
}
// Write a packet into a file descriptor.
int writePacket(int fd, MCPacket *packet)
{
    char varintPacketSize[4];
    int varintPacketSizeLenght = serializeIntoVarint(packet->lenght, varintPacketSize);

    char varintPacketid[4];
    int varintPacketidLenght = serializeIntoVarint(packet->id, varintPacketid);

    int packetSize = varintPacketSizeLenght + varintPacketidLenght + packet->dataSize;
    char *payload = malloc(packetSize);

    memcpy(payload, varintPacketSize, varintPacketSizeLenght);
    memcpy(&payload[varintPacketSizeLenght], varintPacketid, varintPacketidLenght);
    memcpy(&payload[varintPacketSizeLenght + varintPacketidLenght], packet->data, packet->dataSize);
    send(fd, payload, packetSize, 0);
}