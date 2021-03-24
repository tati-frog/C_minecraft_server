#ifndef _BUFFER
#define _BUFFER

#include <stdlib.h>
#include <string.h>

#include "buffer.h"

// Create the buffer object.
Buffer* createBuffer()
{
    Buffer *buffer = malloc(sizeof(Buffer));

    buffer->size = 0;
    buffer->data = malloc(0);
    return buffer;
}
// Read n bytes from the buffer into dest.
int readBuffer(Buffer *buffer, char *dest, int n)
{
    memcpy(dest, buffer->data, n);
    memmove(buffer->data, &buffer->data[n], buffer->size - n);

    buffer->size -= n;
    buffer->data = realloc(buffer->data, buffer->size);
    return 0;
}
// Write data to a buffer.
int writeBuffer(Buffer *buffer, char *data, int n)
{
    buffer->data = realloc(buffer->data, buffer->size + n);
    memcpy(&buffer->data[buffer->size], data, n);
    buffer->size += n;
}
// Release buffer resources and delete the object.
int releaseBuffer(Buffer *buffer)
{
    free(buffer->data);
    free(buffer);
}

#endif