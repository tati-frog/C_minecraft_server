#ifndef _MCPROTOCOL
#define _MCPROTOCOL

#include <stdint.h>
#include "utils/buffer.h"

/****************************/
/*        Data types        */
/****************************/
typedef int8_t mc_boolean;
typedef int8_t mc_byte;
typedef uint8_t mc_ubyte;
typedef int16_t mc_short;
typedef uint16_t mc_ushort;
typedef int32_t mc_int;
typedef int64_t mc_long;
typedef float mc_float;
typedef double mc_double;
typedef struct {
    mc_int size;
    char *data;
} mc_string;

/****************************/
/*          Packets         */
/****************************/
typedef struct{
    mc_int lenght;
    mc_int id;

    int dataSize;
    mc_byte *data;
} MCPacket;

// Read a varint from a file descriptor.
int readVarint(Buffer *buffer, mc_int *buf);
// Serialize a value into a varint and save the byte array into buf.
int serializeIntoVarint(mc_int value, char *buf);

// Read a string from a file descriptor. The string is saved on the heap.
int readString(Buffer *buffer, mc_string *buf);
// Write a string into a file descriptor.
int writeString(Buffer *buffer, char *string);

// Read a packet from a file descriptor. The data is allocated in the heap.
int readPacket(Buffer *buffer, MCPacket *buf);
// Write a packet into a file descriptor.
int writePacket(Buffer *buffer, MCPacket *packet);
// Release resources of a packet.
void releasePacket(MCPacket *packet);
#endif