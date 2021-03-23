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

// Read a string from a file descriptor. The string is saved on the heap.
mc_string readString(int fd);


#endif