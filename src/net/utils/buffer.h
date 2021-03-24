typedef struct {
    int size;
    char *data;
} Buffer;

// Create the dynamic buffer object.
Buffer *createBuffer();
// Read the data from a buffer.
int readBuffer(Buffer *buffer, char *dest, int size);
// Write data to a buffer.
int writeBuffer(Buffer *buffer, char *data, int size);
// Release buffer resources and delete the object.
int releaseBuffer(Buffer *buffer);
