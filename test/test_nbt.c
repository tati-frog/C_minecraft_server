#include <string.h>

#include <criterion/criterion.h>

#include "nbt/nbt.h"
#include "utils/buffer.h"

Test(nbt, test_create_end_tag)
{
    NBT_Tag *endTag = nbtTagCreate(TAG_END, NULL, (NBTPayload)0);

    cr_assert(endTag->tagType == 0);
    cr_assert(endTag->name.length == 0);
    cr_assert(endTag->name.payload == 0);
}


Test(nbt, test_create_byte_tag)
{
    NBT_Tag *byteTag = nbtTagCreate(TAG_BYTE, "test1", (NBTPayload)32);

    cr_assert(byteTag->tagType == TAG_BYTE);
    cr_assert(byteTag->payload.tag_byte == 32);
    cr_assert(byteTag->name.length == 5);
    cr_assert(0 == memcmp(byteTag->name.payload, "test1", 5));
}


Test(nbt, test_create_short_tag)
{
    NBT_Tag *shortTag = nbtTagCreate(TAG_SHORT, "test1", (NBTPayload)4543);

    cr_assert(shortTag->tagType == TAG_SHORT);
    cr_assert(shortTag->payload.tag_short == 4543);
    cr_assert(shortTag->name.length == 5);
    cr_assert(0 == memcmp(shortTag->name.payload, "test1", 5));
}


Test(nbt, test_create_int_tag)
{
    NBT_Tag *intTag = nbtTagCreate(TAG_INT, "test1", (NBTPayload)324234);

    cr_assert(intTag ->tagType == TAG_INT);
    cr_assert(intTag->payload.tag_int == 324234);
    cr_assert(intTag->name.length == 5);
    cr_assert(0 == memcmp(intTag->name.payload, "test1", 5));
}


Test(nbt, test_create_long_tag)
{
    NBT_Tag *longTag = nbtTagCreate(TAG_LONG, "test1", (NBTPayload)10000000000000L);

    cr_assert(longTag->tagType == TAG_LONG);
    cr_assert(longTag->payload.tag_long == 10000000000000L);
    cr_assert(longTag->name.length == 5);
    cr_assert(0 == memcmp(longTag->name.payload, "test1", 5));
}


Test(nbt, test_create_float_tag)
{
    NBT_Tag *floatTag = nbtTagCreate(TAG_FLOAT, "test1", (NBTPayload)0.5346f);

    cr_assert(floatTag->tagType == TAG_FLOAT);
    cr_assert(floatTag->payload.tag_float == 0.5346f);
    cr_assert(floatTag->name.length == 5);
    cr_assert(0 == memcmp(floatTag->name.payload, "test1", 5));
}


Test(nbt, test_create_double_tag)
{
    NBT_Tag *doubleTag = nbtTagCreate(TAG_DOUBLE, "test1", (NBTPayload)0.5346);

    cr_assert(doubleTag->tagType == TAG_DOUBLE);
    cr_assert(doubleTag->payload.tag_double == 0.5346);
    cr_assert(doubleTag->name.length == 5);
    cr_assert(0 == memcmp(doubleTag->name.payload, "test1", 5));
}


Test(nbt, test_create_bytearray_tag)
{
    NBTBytearray bytearrayPayload = nbtBytearrayCreate(12, "Hello world");
    NBT_Tag *bytearrayTag = nbtTagCreate(TAG_BYTE_ARRAY, "test1", (NBTPayload)bytearrayPayload);

    cr_assert(bytearrayTag->tagType == TAG_BYTE_ARRAY);
    cr_assert(0 == memcmp(bytearrayTag->payload.tag_bytearray.payload, "Hello world", 12));
    cr_assert(bytearrayTag->payload.tag_bytearray.size == 12);
    cr_assert(bytearrayTag->name.length == 5);
    cr_assert(0 == memcmp(bytearrayTag->name.payload, "test1", 5));
}


Test(nbt, test_create_string_tag)
{
    NBTString string = nbtStringCreate("Hello world");
    NBT_Tag *stringTag = nbtTagCreate(TAG_STRING, "test1", (NBTPayload)string);

    cr_assert(stringTag->tagType == TAG_STRING);
    cr_assert(0 == memcmp(stringTag->payload.tag_string.payload, "Hello world", 11));
    cr_assert(stringTag->payload.tag_string.length == 11);
    cr_assert(stringTag->name.length == 5);
    cr_assert(0 == memcmp(stringTag->name.payload, "test1", 5));
}


Test(nbt, test_create_list_tag)
{
    NBT_Tag* listTag;
    NBTList listPayload = nbtListCreate(TAG_INT, 10);

    nbtListAddElement(&listPayload, (NBTPayload) 21);
    nbtListAddElement(&listPayload, (NBTPayload) 43);
    listTag = nbtTagCreate(TAG_LIST, "test1", (NBTPayload) listPayload);

    cr_assert(listTag->tagType == TAG_LIST);
    cr_assert(listTag->payload.tag_list.length == 10);
    cr_assert(listTag->payload.tag_list.payloadType == TAG_INT);

    cr_assert(listTag->payload.tag_list.payload[0].tag_int == 21);
    cr_assert(listTag->payload.tag_list.payload[1].tag_int == 43);

    cr_assert(listTag->name.length == 5);
    cr_assert(0 == memcmp(listTag->name.payload, "test1", 5));
}


Test(nbt, test_create_compound_tag)
{
    NBT_Tag* tagCompoundTag;
    NBT_TagCompound tagCompound = nbtTagCompoundCreate();
    
    cr_assert(tagCompound[0].tagType == TAG_END);

    NBT_Tag *intTag = nbtTagCreate(TAG_INT, "test2", (NBTPayload) 54);
    nbtTagCompoundAddTag(&tagCompound, intTag);

    cr_assert(tagCompound[0].payload.tag_int == 54);
    cr_assert(tagCompound[0].tagType == TAG_INT);
    cr_assert(tagCompound[1].tagType == TAG_END);

    tagCompoundTag = nbtTagCreate(TAG_COMPOUND, "test1", (NBTPayload) tagCompound);
    cr_assert(tagCompoundTag->tagType == TAG_COMPOUND);
    cr_assert(tagCompoundTag->name.length == 5);
    cr_assert(0 == memcmp(tagCompoundTag->name.payload, "test1", 5));
}



Test(nbt, test_serialize_tag)
{
    Buffer *testBuffer = createBuffer();

    NBT_TagCompound tagCompound = nbtTagCompoundCreate();
    NBT_Tag* tagCompoundTag;
    
    nbtTagCompoundAddTag(&tagCompound, nbtTagCreate(TAG_INT, "int", (NBTPayload)12345));
    nbtTagCompoundAddTag(&tagCompound, nbtTagCreate(TAG_STRING, "Name", (NBTPayload)nbtStringCreate("This is a test")));
    
    NBTList listPayload = nbtListCreate(TAG_INT, 3);
    NBT_Tag* listTag;

    nbtListAddElement(&listPayload, (NBTPayload)1);
    nbtListAddElement(&listPayload, (NBTPayload)2);
    nbtListAddElement(&listPayload, (NBTPayload)3);

    listTag = nbtTagCreate(TAG_LIST, "List", (NBTPayload)listPayload);
    nbtTagCompoundAddTag(&tagCompound, listTag);

    tagCompoundTag = nbtTagCreate(TAG_COMPOUND, "root", (NBTPayload)tagCompound);
    
    nbtWriteTagInBuffer(tagCompoundTag, testBuffer);

    char data[500];
    readBuffer(testBuffer, data, 500);

    cr_assert(0 == memcmp(data, "\x0a\x00\x04root"
                                "\x03\x00\x03int\x00\x00\x30\x39"
                                "\x08\x00\x04Name\x00\x0eThis is a test"
                                "\x09\x00\x04List\x03\x00\x00\x00\x03\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x03\x00", 65));
}



Test(nbt, test_deserialize_tag)
{
    Buffer *serializedTag = createBuffer();

    writeBuffer(serializedTag, "\x0a\x00\x04root"
                                "\x03\x00\x03int\x00\x00\x30\x39"
                                "\x08\x00\x04Name\x00\x0eThis is a test"
                                "\x09\x00\x04List\x03\x00\x00\x00\x03\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x03\x00", 65);

    NBT_Tag *tag = nbtReadTagFromBuffer(serializedTag);
    
    cr_assert(tag->name.length == 4);
    cr_assert(0 == memcmp(tag->name.payload, "root", 4));

    cr_assert(tag->tagType == TAG_COMPOUND);

    cr_assert(tag->payload.tag_compound[0].tagType == TAG_INT);
    cr_assert(tag->payload.tag_compound[0].name.length == 3);
    cr_assert(0 == memcmp(tag->payload.tag_compound[0].name.payload, "int", 3));
    cr_assert(tag->payload.tag_compound[0].payload.tag_int == 12345);

    cr_assert(tag->payload.tag_compound[1].tagType == TAG_STRING);
    cr_assert(tag->payload.tag_compound[1].name.length == 4);
    cr_assert(0 == memcmp(tag->payload.tag_compound[1].name.payload, "Name", 4));
    cr_assert(tag->payload.tag_compound[1].payload.tag_string.length == 14);
    cr_assert(0 == memcmp(tag->payload.tag_compound[1].payload.tag_string.payload, "This is a test", 14));

    NBT_Tag taglist = tag->payload.tag_compound[2];
    cr_assert(taglist.tagType == TAG_LIST);
    cr_assert(taglist.name.length == 4);
    cr_assert(0 == memcmp(taglist.name.payload, "List", 4));
    cr_assert(taglist.payload.tag_list.payloadType == TAG_INT);
    cr_assert(taglist.payload.tag_list.length == 3);
    cr_assert(taglist.payload.tag_list.payload[0].tag_int == 1);
    cr_assert(taglist.payload.tag_list.payload[1].tag_int == 2);
    cr_assert(taglist.payload.tag_list.payload[2].tag_int == 3);

    cr_assert(tag->payload.tag_compound[3].tagType == TAG_END);

}

