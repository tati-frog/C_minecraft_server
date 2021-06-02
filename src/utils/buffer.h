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
void readAndSaveInFd(Buffer *buffer, int fd, int n);
// Write data to a buffer.
int writeBuffer(Buffer *buffer, char *data, int size);
// Write data from a file decriptor.
int writeBufferFromFd(Buffer *Buffer, int fd, int size);
// Move n bytes from buffer src to buffer dst
int moveDataBetweenBuffers(Buffer *dst, Buffer *src, int n);
// Release all the data from the buffer, without deleting the buffer object
void releaseBufferData(Buffer *buffer);
// Release buffer resources and delete the object.
void releaseBuffer(Buffer *buffer);

#endif