#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>

#include "mcprotocol.h"
#include "./utils/buffer.h"

// Read a varint from a buffer.
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
            return -1;

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

// Read a string from a buffer. The string is saved on the heap.
int readString(Buffer *buffer, mc_string buf)
{
    int readedBytes;
    mc_int length;

    readedBytes = readVarint(buffer, &length);
    if (readedBytes == -1)
        return -1;

    buf = malloc(length + 1);
    buf[length] = 0;

    readedBytes += readBuffer(buffer, buf, length);

    return readedBytes;
}

int writeString(Buffer *buffer, mc_string string)
{
    char stringSizeVarintEncoded[4];
    int stringSizeLenght = serializeIntoVarint(strlen(string), stringSizeVarintEncoded);

    writeBuffer(buffer, stringSizeVarintEncoded, stringSizeLenght);
    writeBuffer(buffer, string, strlen(string));
}

// Initialize a packet structure.
int createPacket(MCPacket *packet)
{
    memset(packet, 0, sizeof(MCPacket));
    packet->data = createBuffer();
}
// Read a packet from a buffer. The data is allocated in the heap.
int readPacket(Buffer *buffer, MCPacket *packet)
{
    mc_int packetSize;
    int packetSizeLength = readVarint(buffer, &packetSize);
    if (packetSize == -1)
        return -1;

    if(packetSize > buffer->size) return -1;

    packet->length = packetSize;
    int packetIdSize = readVarint(buffer, &packet->id);
    if (packetIdSize == -1)
        return -1;

    int readed = moveDataBetweenBuffers(packet->data, buffer,  packet->length - packetIdSize);

    return readed;
}
// Write a packet into a buffer.
int writePacket(Buffer *buffer, MCPacket *packet)
{
    char varintPacketid[4];
    memset(varintPacketid, 0, 4);

    int varintPacketidLength = serializeIntoVarint(packet->id, varintPacketid);

    packet->length = packet->data->size + varintPacketidLength;

    char varintPacketSize[4];
    memset(varintPacketSize, 0, 4);
    int varintPacketSizeLength = serializeIntoVarint(packet->length, varintPacketSize);

    writeBuffer(buffer, varintPacketSize, varintPacketSizeLength);
    writeBuffer(buffer, varintPacketid, varintPacketidLength);
    moveDataBetweenBuffers(buffer, packet->data, packet->data->size);
}
// Release resources of a packet.
void releasePacket(MCPacket *packet)
{
    if (packet->data == NULL)
        return;
    releaseBuffer(packet->data);
}


int readHandshakingPacket(MCPacket *inputPacket, in_HandshakePacket *packet)
{
    readVarint(inputPacket->data, &packet->protocolVersion);
    readString(inputPacket->data, packet->serverAddress);
    readBuffer(inputPacket->data, (char *)&packet->serverPort, sizeof(mc_ushort));
    readVarint(inputPacket->data, &packet->nextState);
}


int readPingPacket(MCPacket *inputPacket, in_PingStatusPacket *packet)
{
    readBuffer(inputPacket->data, (char*)&packet->payload, sizeof(mc_long));
}

int writePongPacket(MCPacket *packet, out_PongStatusPacket *pongPacket)
{
    packet->id = PONG;

    writeBuffer(packet->data, (char *)&pongPacket->payload, sizeof(mc_long));
}

int writeStatusResponsePacket(MCPacket *packet, out_ResponseStatusPacket *statusResponse)
{
    packet->id = RESPONSE;

    writeString(packet->data, statusResponse->jsonResponse);
}