typedef struct {
    int size;
    char *data;
} DynamicBuffer;

// Create the dynamic buffer object.
DynamicBuffer *createDynamicBuffer();
// Read the data from a buffer.
int readBuffer(DynamicBuffer *buffer, char *dest, int size);
// Write data to a buffer.
int writeBuffer(DynamicBuffer *buffer, char *data, int size);
// Release buffer resources and delete the object.
int releaseBuffer(DynamicBuffer *buffer);
