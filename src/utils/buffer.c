#include <unistd.h>
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
    if(n > buffer->size) n = buffer->size;

    memcpy(dest, buffer->data, n);
    memmove(buffer->data, &buffer->data[n], buffer->size - n);

    buffer->size -= n;
    buffer->data = realloc(buffer->data, buffer->size);
    return n;
}
// Read data from buffer and send it to a file descriptor.
int readAndSaveInFd(Buffer *buffer, int fd, int n)
{
    char tmp[n];
    readBuffer(buffer, tmp, n);
    write(fd, tmp, n);
}
// Write data to a buffer.
int writeBuffer(Buffer *buffer, char *data, int n)
{
    buffer->data = realloc(buffer->data, buffer->size + n);
    memcpy(&buffer->data[buffer->size], data, n);
    buffer->size += n;

    return n;
}
// Write data from a file decriptor.
int writeBufferFromFd(Buffer *buffer, int fd, int n)
{
    buffer->data = realloc(buffer->data, buffer->size + n);
    read(fd, buffer->data, n);
    buffer->size += n;

    return n;
}
// Move n bytes from buffer src to buffer dst
int moveDataBetweenBuffers(Buffer *dst, Buffer *src, int n)
{
    char temp[n];
    readBuffer(src, temp, n);
    writeBuffer(dst, temp, n);

    return n;
}
// Release buffer resources and delete the object.
int releaseBuffer(Buffer *buffer)
{
    free(buffer->data);
    free(buffer);
}
