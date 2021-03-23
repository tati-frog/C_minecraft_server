#include <sys/socket.h>

#include "mcprotocol.h"

// Read a varint from a file descriptor.
mc_int readVarint(int fd)
{
    int numRead = 0;
    int result = 0;
    char read;
    do {
        recv(fd, &read, 1, 0);
        int value = (read & 0b01111111);
        result |= (value << (7 * numRead));

        numRead++;
        if (numRead > 5) {
            
        }
    } while ((read & 0b10000000) != 0);

    return result;
}

// Read a string from a file descriptor. The string is saved on the heap.
mc_string readString(int fd)
{

}