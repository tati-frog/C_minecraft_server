// TODO Create method for deallocate memory for nbt tags.
#ifndef _NBT
#define _NBT

#include <stdint.h>

#include "utils/buffer.h"

/**
 * @brief The datatype of the NBT tag.
 * 
 */
typedef enum {
    TAG_END=0,
    TAG_BYTE=1,
    TAG_SHORT=2,
    TAG_INT=3,
    TAG_LONG=4,
    TAG_FLOAT=5,
    TAG_DOUBLE=6,
    TAG_BYTE_ARRAY=7,
    TAG_STRING=8,
    TAG_LIST=9,
    TAG_COMPOUND=10,
} NBT_TagType;

typedef union _NBTPayload NBTPayload;

typedef struct{
    uint16_t length;
    char *payload;
} NBTString;

typedef struct {
    int32_t size;
    int8_t *payload;
} NBTBytearray;

typedef struct {
    NBT_TagType payloadType;
    int32_t length;

    NBTPayload *payload;
} NBTList;

typedef struct _NBT_Tag NBT_Tag;
typedef NBT_Tag* NBT_TagCompound;


/**
 * @brief Union representing the payload that can contain a tag.
 * 
 */
typedef union _NBTPayload{
        int8_t tag_byte;
        int16_t tag_short;
        int32_t tag_int;
        int64_t tag_long;
        float tag_float;
        double tag_double;

        NBTBytearray tag_bytearray;
        NBTString tag_string;
        NBTList tag_list;
        NBT_TagCompound tag_compound;
} NBTPayload;

typedef struct _NBT_Tag {
    NBT_TagType tagType;
    NBTString name;

    NBTPayload payload;
} NBT_Tag;

NBTString nbtStringCreate(char* payload);
NBTBytearray nbtBytearrayCreate(int size, char* payload);

NBTList nbtListCreate(NBT_TagType elementType, int numberOfElements);
void nbtListAddElement(NBTList* list, NBTPayload element);

NBT_TagCompound nbtTagCompoundCreate();
int nbtTagCompoundAddTag(NBT_TagCompound* tagCompound, NBT_Tag *tag);

/**
 * @brief Create a tag object.
 * 
 * @param type The data type of the tag.
 * @param name The name of the tag.
 * @param payload The payload of the tag.
 * @return NBT_Tag* 
 */
NBT_Tag *nbtTagCreate(NBT_TagType type, char *name, NBTPayload payload);


// Serialization and deserialization

/**
 * @brief Serialize and write the tag into a buffer.
 * 
 * @param tag The tag to serialize.
 * @param buffer The buffer to write to.
 * @return int 
 */
int nbtWriteTagInBuffer(NBT_Tag *tag, Buffer *buffer);
/**
 * @brief Read and deserialize the tag from a buffer.
 * 
 * @param buffer The buffer to read from.
 * @return NBT_Tag* The new tag.
 */
NBT_Tag *nbtReadTagFromBuffer(Buffer *buffer);

#endif
