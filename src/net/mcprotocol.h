#ifndef _MCPROTOCOL
#define _MCPROTOCOL

#include <stdint.h>

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

    mc_byte *data;
} Packet;

// Read a varint from a file descriptor.
int readVarint(int fd, mc_int *buf);
// Write a varint into a file descriptor.
int writeVarint(int fd, mc_int value);

// Read a string from a file descriptor. The string is saved on the heap.
int readString(int fd, mc_string *buf);
// Write a string into a file descriptor.
int writeString(int fd, char *string);

#endif