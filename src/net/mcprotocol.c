#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mcprotocol.h"
#include "utils/buffer.h"
#include <uuid/uuid.h>

// Read a varint from a buffer.
int mcVarintRead(Buffer *buffer, mc_int *buf)
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
            *buf = 0;
            return -1;
        }

    } while ((read & 0b10000000) != 0);

    *buf = result;

    return numRead;
}
// Serialize a value into a varint and save the byte array into buf.
int mcVarintWrite(mc_int value, char *buf)
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
int mcStringRead(Buffer *buffer, mc_string *buf)
{
    int readedBytes;
    mc_int length;

    readedBytes = mcVarintRead(buffer, &length);
    if (readedBytes == -1)
        return -1;

    *buf = malloc(length + 1);
    (*buf)[length] = 0;

    readedBytes += readBuffer(buffer, *buf, length);

    return readedBytes;
}

void mcStringWrite(Buffer *buffer, mc_string string)
{
    char stringSizeVarintEncoded[4];
    int stringSizeLenght = mcVarintWrite(strlen(string), stringSizeVarintEncoded);

    writeBuffer(buffer, stringSizeVarintEncoded, stringSizeLenght);
    writeBuffer(buffer, string, strlen(string));
}

// Initialize a packet structure.
void mcPacketCreate(MCPacket* packet)
{
    memset(packet, 0, sizeof(MCPacket));
    packet->data = createBuffer();
}
// Read a packet from a buffer. The data is allocated in the heap.
int mcPacketRead(Buffer *buffer, MCPacket *packet)
{
    mcPacketCreate(packet);

    mc_int packetSize;
    mcVarintRead(buffer, &packetSize);
    if (packetSize == -1)
        return -1;

    if (packetSize > buffer->size)
        return -1;

    packet->length = packetSize;
    int packetIdSize = mcVarintRead(buffer, &packet->id);
    if (packetIdSize == -1)
        return -1;

    int readed = moveDataBetweenBuffers(packet->data, buffer, packet->length - packetIdSize);

    return readed;
}
// Write a packet into a buffer.
void mcPacketWrite(Buffer *buffer, MCPacket *packet)
{
    char varintPacketid[4];
    memset(varintPacketid, 0, 4);

    int varintPacketidLength = mcVarintWrite(packet->id, varintPacketid);

    packet->length = packet->data->size + varintPacketidLength;

    char varintPacketSize[4];
    memset(varintPacketSize, 0, 4);
    int varintPacketSizeLength = mcVarintWrite(packet->length, varintPacketSize);

    writeBuffer(buffer, varintPacketSize, varintPacketSizeLength);
    writeBuffer(buffer, varintPacketid, varintPacketidLength);
    moveDataBetweenBuffers(buffer, packet->data, packet->data->size);
}
// Release resources of a packet.
void mcPacketDestroy(MCPacket* packet)
{
    if (packet->data == NULL)
        return;
    releaseBuffer(packet->data);
}

void readHandshakingPacket(MCPacket* inputPacket, in_HandshakePacket *packet)
{
    mcVarintRead(inputPacket->data, &packet->protocolVersion);
    mcStringRead(inputPacket->data, &packet->serverAddress);
    readBuffer(inputPacket->data, (char *)&packet->serverPort, sizeof(mc_ushort));
    mcVarintRead(inputPacket->data, &packet->nextState);
}

void readPingPacket(MCPacket* inputPacket, in_PingStatusPacket *packet)
{
    readBuffer(inputPacket->data, (char *)&packet->payload, sizeof(mc_long));
}

void writePongPacket(MCPacket* packet, out_PongStatusPacket *pongPacket)
{
    mcPacketCreate(packet);
    packet->id = STATUS_PONG;

    writeBuffer(packet->data, (char *)&pongPacket->payload, sizeof(mc_long));
}

void writeStatusResponsePacket(MCPacket* packet, out_ResponseStatusPacket *statusResponse)
{
    mcPacketCreate(packet);
    packet->id = STATUS_RESPONSE;

    mcStringWrite(packet->data, statusResponse->jsonResponse);
}

void readLoginStart(MCPacket* inputPacket, in_LoginStartPacket *packet)
{
    mcStringRead(inputPacket->data, &packet->name);
}

void writeLoginSuccess(MCPacket* packet, out_LoginSuccessPacket *loginSuccessPacket)
{
    mcPacketCreate(packet);
    packet->id = LOGIN_SUCCESS;

    mcStringWrite(packet->data, loginSuccessPacket->uuid);
    mcStringWrite(packet->data, loginSuccessPacket->username);
}

void readClientSettings(MCPacket* inputPacket, in_ClientSettingsPacket *packet)
{
    mcStringRead(inputPacket->data, &packet->locale);
    readBuffer(inputPacket->data, (char *)&packet->viewDistance, sizeof(mc_byte));
    mcVarintRead(inputPacket->data, &packet->chatMode);
    readBuffer(inputPacket->data, (char *)&packet->chatColors, sizeof(mc_boolean));
    readBuffer(inputPacket->data, (char *)&packet->displayedSkinParts, sizeof(mc_ubyte));
    mcVarintRead(inputPacket->data, &packet->mainHand);
}

void writeJoinGame(MCPacket* packet, out_JoinGamePacket *joinGamePacket)
{
    mcPacketCreate(packet);
    packet->id = PLAY_JOIN_GAME;

    writeBuffer(packet->data, (char *)&joinGamePacket->entityId, sizeof(mc_int));
    writeBuffer(packet->data, (char *)&joinGamePacket->gamemode, sizeof(mc_ubyte));
    writeBuffer(packet->data, (char *)&joinGamePacket->dimension, sizeof(mc_int));
    writeBuffer(packet->data, (char *)&joinGamePacket->maxPlayers, sizeof(mc_ubyte));
    mcStringWrite(packet->data, joinGamePacket->levelType);
    char viewDistanceEncoded[4];
    int sizeofViewdistance = mcVarintWrite(joinGamePacket->viewDistance, viewDistanceEncoded);
    writeBuffer(packet->data, viewDistanceEncoded, sizeofViewdistance);
    writeBuffer(packet->data, (char *)&joinGamePacket->reducedDebugInfo, sizeof(mc_boolean));
}

void writeChunkData(MCPacket* packet, out_ChunkData *chunkDataPacket)
{
    mcPacketCreate(packet);
    packet->id = PLAY_CHUNK_DATA;
    
    writeBuffer(packet->data, (char*)&chunkDataPacket->chunkX, sizeof(mc_int));
    writeBuffer(packet->data, (char*)&chunkDataPacket->chunkZ, sizeof(mc_int));
    writeBuffer(packet->data, (char*)&chunkDataPacket->fullChunk, sizeof(mc_boolean));
    char primaryBitMaskEncoded[4];
    int sizeofPrimaryBitMask = mcVarintWrite(chunkDataPacket->primaryBitMask, primaryBitMaskEncoded);
    writeBuffer(packet->data, primaryBitMaskEncoded, sizeofPrimaryBitMask);
    char sizeEncoded[4];
    int sizeofSize = mcVarintWrite(chunkDataPacket->size, sizeEncoded);
    writeBuffer(packet->data, (char*)sizeEncoded, sizeofSize);
    writeBuffer(packet->data, (char*)chunkDataPacket->data, chunkDataPacket->size);
}

void writePlayerPositionAndLook(MCPacket* packet, out_PlayerPositionAndLook* playerPositionAndLookPacket)
{
    mcPacketCreate(packet);
    packet->id = PLAY_PLAYER_POSITION_AND_LOOK_OUTBOUND;

    writeBuffer(packet->data, (char*)&playerPositionAndLookPacket->x, sizeof(mc_double));
    writeBuffer(packet->data, (char*)&playerPositionAndLookPacket->y, sizeof(mc_double));
    writeBuffer(packet->data, (char*)&playerPositionAndLookPacket->z, sizeof(mc_double));
    writeBuffer(packet->data, (char*)&playerPositionAndLookPacket->yaw, sizeof(mc_float));
    writeBuffer(packet->data, (char*)&playerPositionAndLookPacket->pitch, sizeof(mc_float));
    writeBuffer(packet->data, (char*)&playerPositionAndLookPacket->flags, 1);
    char teleportIdEncoded[4];
    int sizeofTeleportId = mcVarintWrite(playerPositionAndLookPacket->teleportId, teleportIdEncoded);
    writeBuffer(packet->data, teleportIdEncoded, sizeofTeleportId);
}
