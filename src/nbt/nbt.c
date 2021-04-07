#include <stdlib.h>
#include <string.h>

#include "nbt.h"
#include "utils/buffer.h"

NBT_Tag *allocateTag()
{
    NBT_Tag *tag = malloc(sizeof(NBT_Tag));
    memset(tag, 0, sizeof(NBT_Tag));
    
    return tag;
}


NBTString createString(char *payload)
{
    NBTString string;
    string.length = strlen(payload);
    string.payload = malloc(string.length);

    memcpy(string.payload, payload, string.length);

    return string;
}

void setTagNameAndType(NBT_Tag *tag, char *name, NBT_TagType type)
{
    if(name != NULL){
        NBTString stringName = createString(name);
        tag->name = stringName;
    }

    tag->tagType = type;
}

NBT_Tag *nbtEndTagCreate()
{
    NBT_Tag *tag = allocateTag();

    tag->name.length = 0;
    tag->name.payload = 0;
    tag->tagType = 0;

    return tag;
}

NBT_Tag *nbtByteTagCreate(char *name, int8_t payload)
{
    NBT_Tag *tag = allocateTag();

    setTagNameAndType(tag, name, TAG_BYTE);
    tag->payload.tag_byte = payload;

    return tag;
}

NBT_Tag *nbtShortTagCreate(char *name, int16_t payload)
{
    NBT_Tag *tag = allocateTag();

    setTagNameAndType(tag, name, TAG_SHORT);
    tag->payload.tag_short = payload;

    return tag;
}

NBT_Tag *nbtIntTagCreate(char *name, int32_t payload)
{
    NBT_Tag *tag = allocateTag();

    setTagNameAndType(tag, name, TAG_INT);
    tag->payload.tag_int = payload;

    return tag;
}

NBT_Tag *nbtLongTagCreate(char *name, int64_t payload)
{
    NBT_Tag *tag = allocateTag();

    setTagNameAndType(tag, name, TAG_LONG);
    tag->payload.tag_long = payload;

    return tag;
}

NBT_Tag *nbtFloatTagCreate(char *name, float payload)
{
     NBT_Tag *tag = allocateTag();

    setTagNameAndType(tag, name, TAG_FLOAT);
    tag->payload.tag_float = payload;

    return tag;
}

NBT_Tag *nbtDoubleTagCreate(char *name, double payload)
{
    NBT_Tag *tag = allocateTag();

    setTagNameAndType(tag, name, TAG_DOUBLE);
    tag->payload.tag_double = payload;

    return tag;
}

NBT_Tag *nbtByteArrayTagCreate(char *name, int length, uint8_t *payload)
{
    NBT_Tag *tag = allocateTag();

    setTagNameAndType(tag, name, TAG_BYTE_ARRAY);

    tag->payload.tag_bytearray.size = length;
    tag->payload.tag_bytearray.payload = malloc(length);

    memcpy(tag->payload.tag_bytearray.payload, payload, length);

    return tag;
}

NBT_Tag *nbtStringTagCreate(char *name, char *payload)
{
    NBT_Tag *tag = allocateTag();

    setTagNameAndType(tag, name, TAG_STRING);

    tag->payload.tag_string.length = strlen(payload);
    tag->payload.tag_string.payload = malloc(tag->payload.tag_string.length);

    memcpy(tag->payload.tag_string.payload, payload, tag->payload.tag_string.length);

    return tag;
}

NBT_Tag *nbtListTagCreate(char *name, NBT_TagType elementType, int size)
{
    NBT_Tag *tag = allocateTag();

    setTagNameAndType(tag, name, TAG_LIST);

    tag->payload.tag_list.length = size;
    tag->payload.tag_list.payloadType = elementType;

    tag->payload.tag_list.payload = malloc(sizeof(NBTPayload) * tag->payload.tag_list.length);
    memset(tag->payload.tag_list.payload, 0, sizeof(NBTPayload) * tag->payload.tag_list.length);

    return tag;
}

void nbtListTagAddElement(NBT_Tag *list, NBTPayload element)
{
    NBTPayload emptyPayload;
    memset(&emptyPayload, 0, sizeof(NBTPayload));

    for(int i = 0; i < list->payload.tag_list.length; ++i)
    {
        if(memcmp(&list->payload.tag_list.payload[i], &emptyPayload, sizeof(NBTPayload)) != 0) continue;
        list->payload.tag_list.payload[i] = element;
        return;
    }
}

NBT_Tag *nbtCompoundTagCreate(char *name)
{
    NBT_Tag *tag = allocateTag();

    setTagNameAndType(tag, name, TAG_COMPOUND);

    tag->payload.tag_compound = malloc(sizeof(NBT_Tag));
    tag->payload.tag_compound[0].tagType = TAG_END;

    return tag;
}
int nbtCompoundTagAddTag(NBT_Tag *compoundTag, NBT_Tag *tag)
{
    for(int i = 0; ; ++i)
    {
        if(compoundTag->payload.tag_compound[i].tagType == TAG_END){
            compoundTag->payload.tag_compound = realloc(compoundTag->payload.tag_compound, sizeof(NBT_Tag) * (i + 2));
            if(compoundTag->payload.tag_compound == NULL) return -1;
            
            compoundTag->payload.tag_compound[i + 1].tagType = TAG_END;

            memcpy(&compoundTag->payload.tag_compound[i], tag, sizeof(NBT_Tag));

            return 0;
        }
    }
}

// TAG serialization

int16_t swapendianess16(int16_t value)
{
    return (((value & 0xFF) << 8) |
            ((value & 0xFF00) >> 8));
}
    
int32_t swapendianess32(int32_t value) 
{
    return (((value & 0x000000FF) << 24) |
            ((value & 0x0000FF00) <<  8) |
            ((value & 0x00FF0000) >>  8) |
            ((value & 0xFF000000) >> 24));
}

int64_t swapendianess64(int64_t value)
{
	return (((value & 0xFF) << 56) |
			((value & 0xFF00) << 24) |
			((value & 0xFF0000) << 8) |
			((value & 0xFF000000) >> 8) |
			((value & 0xFF00000000) >> 24) |
			((value & 0xFF0000000000) >> 56));
}

void serializePayload(NBTPayload *payload, NBT_TagType type, Buffer *buffer)
{
    switch(type)
    {
    case TAG_BYTE:
    {
        writeBuffer(buffer, (char *)&payload->tag_byte, 1);
    }break;
    
    case TAG_SHORT:
    {
		int16_t swap = swapendianess16(payload->tag_short);
        writeBuffer(buffer, (char *)&swap, 2);
    }break;

    case TAG_INT:
    {
        int32_t swap = swapendianess32(payload->tag_int);
        writeBuffer(buffer, (char *)&swap, 4);
    }break;

    case TAG_LONG:
    {
        int64_t swap = swapendianess64(payload->tag_long);
        writeBuffer(buffer, (char *)&swap, 8);
    }break;

    case TAG_FLOAT:
    {
        writeBuffer(buffer, (char *)&payload->tag_float, 4);
    }break;

    case TAG_DOUBLE:
    {
        writeBuffer(buffer, (char *)&payload->tag_double, 8);
    }break;
    
    case TAG_BYTE_ARRAY:
    {
        writeBuffer(buffer, (char *)payload->tag_bytearray.payload, payload->tag_bytearray.size);
    }break;

    case TAG_STRING:
    {
        int16_t lengthSwap = swapendianess16(payload->tag_string.length);
        writeBuffer(buffer, (char *)&lengthSwap, 2);
        writeBuffer(buffer, payload->tag_string.payload, payload->tag_string.length);
    }break;

    case TAG_LIST:
    {
        writeBuffer(buffer, (char *)&payload->tag_list.payloadType, 1);

        int32_t listLengthSwap = swapendianess32(payload->tag_list.length);
        writeBuffer(buffer, (char *)&listLengthSwap, 4);

        for(int i = 0; i < payload->tag_list.length; ++i)
        {
            serializePayload(&payload->tag_list.payload[i], payload->tag_list.payloadType, buffer);
        }
    }break;

    case TAG_COMPOUND:
    {
        for(int i = 0;; ++i)
        {
            NBT_Tag tag = payload->tag_compound[i];

            if(tag.tagType == TAG_END){
                writeBuffer(buffer, "\x00", 1);
                return;
            }

            writeBuffer(buffer, (char *)&tag.tagType, 1);
            int16_t nameLengthSwap = swapendianess16(tag.name.length);
            writeBuffer(buffer, (char *)&nameLengthSwap, 2);
            writeBuffer(buffer, tag.name.payload, tag.name.length);
            
            serializePayload(&tag.payload, tag.tagType, buffer);
        }
    }break;

    case TAG_END:
    {
        writeBuffer(buffer, "\x00", 1);
    }break;

    }
}

int nbtWriteTagInBuffer(NBT_Tag *tag, Buffer *buffer)
{
    writeBuffer(buffer, (char *)&tag->tagType, 1);
    int16_t nameLengthSwap = swapendianess16(tag->name.length);
    writeBuffer(buffer, (char *)&nameLengthSwap, 2);
    writeBuffer(buffer, tag->name.payload, tag->name.length);

    serializePayload(&tag->payload, tag->tagType, buffer);
    return 0;
}

void deserializePayload(Buffer *buf, NBT_TagType type, NBTPayload *payload)
{
    switch(type)
    {
    case TAG_BYTE:
    {
        readBuffer(buf, &payload->tag_byte, 1);
    }break;
    
    case TAG_SHORT:
    {
		readBuffer(buf, &payload->tag_short, 2);
        payload->tag_short = swapendianess16(payload->tag_short);
    }break;

    case TAG_INT:
    {
        readBuffer(buf, &payload->tag_int, 4);
        payload->tag_int = swapendianess32(payload->tag_int);
    }break;

    case TAG_LONG:
    {
        readBuffer(buf, &payload->tag_long, 8);
        payload->tag_long = swapendianess64(payload->tag_long);
    }break;

    case TAG_FLOAT:
    {
        readBuffer(buf, &payload->tag_float, 4);
    }break;

    case TAG_DOUBLE:
    {
        readBuffer(buf, &payload->tag_double, 8);
    }break;
    
    case TAG_BYTE_ARRAY:
    {
        int32_t length;
        readBuffer(buf, &length, 4);
        length = swapendianess32(length);
        payload->tag_bytearray.size = length;

        payload->tag_bytearray.payload = malloc(length);

        readBuffer(buf, payload->tag_bytearray.payload, length);
    }break;

    case TAG_STRING:
    {
        int16_t length;
        readBuffer(buf, &length, 2);
        length = swapendianess16(length);
        payload->tag_string.length = length;

        payload->tag_string.payload = malloc(length + 1);
        memset(payload->tag_string.payload, 0, length + 1);

        readBuffer(buf, payload->tag_string.payload, length);
    }break;

    case TAG_LIST:
    {
        NBT_TagType listType = 0;
        int32_t length;

        readBuffer(buf, &listType, 1);
        readBuffer(buf, &length, 4);
        length = swapendianess32(length);

        payload->tag_list.payloadType = listType;
        payload->tag_list.length = length;

        payload->tag_list.payload = malloc(sizeof(NBTPayload) * length);

        for(int i = 0; i < length; ++i)
        {
            deserializePayload(buf, listType , &payload->tag_list.payload[i]);
        }
    }break;

    case TAG_COMPOUND:
    {
        payload->tag_compound = malloc(0);
        for(int i = 0;; ++i)
        {
            NBT_Tag *tag = nbtReadTagFromBuffer(buf);
            payload->tag_compound = realloc(payload->tag_compound, (i + 1) * sizeof(NBT_Tag));

            memcpy(&payload->tag_compound[i], tag, sizeof(NBT_Tag));

            if(tag->tagType == TAG_END) break;
        }
    }break;
    }
}

NBT_Tag *nbtReadTagFromBuffer(Buffer *buffer)
{
    NBT_Tag *tag = allocateTag();

    NBT_TagType tagtype = 0;
    readBuffer(buffer, &tagtype, 1);
    tag->tagType = tagtype;

    if(tagtype == TAG_END) return tag;

    int16_t nameLength;
    char *name;
    readBuffer(buffer, &nameLength, 2);
    nameLength = swapendianess16(nameLength);

    name = malloc(nameLength + 1);
    memset(name, 0, nameLength + 1);

    readBuffer(buffer, name, nameLength);
    setTagNameAndType(tag, name, tagtype);

    NBTPayload payload;
    deserializePayload(buffer, tagtype, &payload);

    tag->payload = payload;
    return tag;
}
