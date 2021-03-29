#ifndef _BUFFER
#define _BUFFER

typedef struct {
    int size;
    char *data;
} Buffer;

// Create the dynamic buffer object.
Buffer *createBuffer();
// Read the data from a buffer.
int readBuffer(Buffer *buffer, char *dest, int size);
// Read data from buffer and send it to a file descriptor.
int readAndSaveInFd(Buffer *buffer, int fd, int n);
// Write data to a buffer.
int writeBuffer(Buffer *buffer, char *data, int size);
// Write data from a file decriptor.
int writeBufferFromFd(Buffer *Buffer, int fd, int size);
// Move n bytes from buffer src to buffer dst
int moveDataBetweenBuffers(Buffer *dst, Buffer *src, int n);
// Release buffer resources and delete the object.
int releaseBuffer(Buffer *buffer);

#endif