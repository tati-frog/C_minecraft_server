/**
 * @file nbt.h
 * @brief Serializing and deserializing NBT objects
 * @version 0.1
 * @date 2021-04-07
 * 
 */

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

typedef struct{
    uint16_t length;
    char *payload;
} NBTString;

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

/**
 * @brief Creates an end tag, this is used when a composite tag ends.
 * 
 * @return NBT_Tag*
 */
NBT_Tag *nbtEndTagCreate();

/**
 * @brief Create a tag with a byte value.
 * 
 * @param name The name of the tag.
 * @param payload The payload.
 * @return NBT_Tag* 
 */
NBT_Tag *nbtByteTagCreate(char *name, int8_t payload);

/**
 * @brief Create a tag with a short value.
 * 
 * @param name The name of the tag.
 * @param payload The payload.
 * @return NBT_Tag* 
 */
NBT_Tag *nbtShortTagCreate(char *name, int16_t payload);

/**
 * @brief Create a tag with an int value.
 * 
 * @param name The name of the tag.
 * @param payload The payload.
 * @return NBT_Tag* 
 */
NBT_Tag *nbtIntTagCreate(char *name, int32_t payload);

/**
 * @brief Create a tag with a long value.
 * 
 * @param name The name of the tag.
 * @param payload The payload.
 * @return NBT_Tag* 
 */
NBT_Tag *nbtLongTagCreate(char *name, int64_t payload);

/**
 * @brief Create a tag with a float value.
 * 
 * @param name The name of the tag.
 * @param payload The payload.
 * @return NBT_Tag* 
 */
NBT_Tag *nbtFloatTagCreate(char *name, float payload);

/**
 * @brief Create a tag with a double value.
 * 
 * @param name The name of the tag.
 * @param payload The payload.
 * @return NBT_Tag* 
 */
NBT_Tag *nbtDoubleTagCreate(char *name, double payload);

/**
 * @brief Create a tag with a bytearray value
 * 
 * @param name The name of the tag.
 * @param length The length of the array.
 * @param payload The payload.
 * @return NBT_Tag* 
 */
NBT_Tag *nbtByteArrayTagCreate(char *name, int length, uint8_t *payload);

/**
 * @brief Create a tag with a string value.
 * 
 * @param name The name of the tag.
 * @param payload The payload.
 * @return NBT_Tag* 
 */
NBT_Tag *nbtStringTagCreate(char *name, char *payload);

/**
 * @brief Create a tag with a list value.
 * 
 * @param name The name of the tag.
 * @param elementType The type of element the list its going to save.
 * @param size The number of elements the list its going to save.
 * @return NBT_Tag* 
 */
NBT_Tag *nbtListTagCreate(char *name, NBT_TagType elementType, int size);
/**
 * @brief Add an element to a list tag.
 * 
 * @param list A pointer to a list tag.
 * @param element The payload to append to the list.
 */
void nbtListTagAddElement(NBT_Tag *list, NBTPayload element);

/**
 * @brief Create a compound tag.
 * 
 * @param name The name of the tag.
 * @return NBT_Tag* 
 */
NBT_Tag *nbtCompoundTagCreate(char *name);
/**
 * @brief Add an element to the compound tag. (The END tag its added automatically)
 * 
 * @param compoundTag A pointer to the compound tag.
 * @param tag The tag to add in the compound tag.
 * @return int 
 */
int nbtCompoundTagAddTag(NBT_Tag *compoundTag, NBT_Tag *tag);


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
