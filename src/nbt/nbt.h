#ifndef _NBT
#define _NBT

#include <stdint.h>

#include "utils/buffer.h"

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

typedef struct _NBT_List NBTList;
typedef struct _NBT_Tag NBT_Tag;

typedef struct{
    uint16_t length;
    char *payload;
} NBTString;

typedef union _NBTPayload{
        int8_t tag_byte;
        int16_t tag_short;
        int32_t tag_int;
        int64_t tag_long;
        float tag_float;
        double tag_double;

        struct {
            int32_t size;
            int8_t *payload;
        } tag_bytearray;

        NBTString tag_string;

        struct {
            NBT_TagType payloadType;
            int32_t length;

            union _NBTPayload *payload;
        } tag_list;

        NBT_Tag *tag_compound;
} NBTPayload;

typedef struct _NBT_Tag {
    NBT_TagType tagType;
    NBTString name;

    NBTPayload payload;
} NBT_Tag;

// TAG objects

NBT_Tag *nbtEndTagCreate();

NBT_Tag *nbtByteTagCreate(char *name, int8_t payload);

NBT_Tag *nbtShortTagCreate(char *name, int16_t payload);

NBT_Tag *nbtIntTagCreate(char *name, int32_t payload);

NBT_Tag *nbtLongTagCreate(char *name, int64_t payload);

NBT_Tag *nbtFloatTagCreate(char *name, float payload);

NBT_Tag *nbtDoubleTagCreate(char *name, double payload);

NBT_Tag *nbtByteArrayTagCreate(char *name, int length, uint8_t *payload);

NBT_Tag *nbtStringTagCreate(char *name, char *payload);

NBT_Tag *nbtListTagCreate(char *name, NBT_TagType elementType, int size);
void nbtListTagAddElement(NBT_Tag *list, NBTPayload element);

NBT_Tag *nbtCompoundTagCreate(char *name);
int nbtCompoundTagAddTag(NBT_Tag *compoundTag, NBT_Tag *tag);


// Serialization and deserialization

int nbtWriteTagInBuffer(NBT_Tag *tag, Buffer *buffer);
NBT_Tag *nbtReadTagFromBuffer(Buffer *buffer);

#endif
