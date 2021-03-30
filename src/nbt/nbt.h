#ifndef _NBT
#define _NBT

#include <stdint.h>

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
    TAG_INT_ARRAY=11
} NBT_TagType;

typedef struct{
    uint16_t length;
    char *payload;
} NBT_String;

typedef struct {
    NBT_TagType tagType;
    NBT_String name;

    void *payload;
} NBT_Tag;

NBT_Tag *nbtEndTagCreate();

NBT_Tag *nbtByteTagCreate(int8_t payload);

NBT_Tag *nbtShortTagCreate(int16_t payload);

NBT_Tag *nbtIntTagCreate(int32_t payload);

NBT_Tag *nbtLongTagCreate(int64_t payload);

NBT_Tag *nbtFloatTagCreate(float payload);

NBT_Tag *nbtDoubleTagCreate(double payload);

NBT_Tag *nbtByteArrayTagCreate(int length, uint8_t *payload);

NBT_Tag *nbtStringTagCreate(short length, char *payload);

NBT_Tag *nbtListTagCreate(NBT_TagType elementType, int size);
int nbtListTagAddElement(void *element);

NBT_Tag *nbtCompundTagCreate();
int nbtCompoundTagAddTag(NBT_Tag *tag);

NBT_Tag *nbtIntArrayTagCrerate();
#endif