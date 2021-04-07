#include <string.h>

#include <check.h>

#include "nbt/nbt.h"
#include "utils/buffer.h"

START_TEST(test_create_end_tag)
{
    NBT_Tag *endTag = nbtEndTagCreate();

    ck_assert(endTag->tagType == 0);
    ck_assert(endTag->name.length == 0);
    ck_assert(endTag->name.payload == 0);
}
END_TEST

START_TEST(test_create_byte_tag)
{
    NBT_Tag *byteTag = nbtByteTagCreate("test1", 32);

    ck_assert(byteTag->tagType == TAG_BYTE);
    ck_assert(byteTag->payload.tag_byte == 32);
    ck_assert(byteTag->name.length == 5);
    ck_assert(0 == memcmp(byteTag->name.payload, "test1", 5));
}
END_TEST

START_TEST(test_create_short_tag)
{
    NBT_Tag *shortTag = nbtShortTagCreate("test1", 4543);

    ck_assert(shortTag->tagType == TAG_SHORT);
    ck_assert(shortTag->payload.tag_short == 4543);
    ck_assert(shortTag->name.length == 5);
    ck_assert(0 == memcmp(shortTag->name.payload, "test1", 5));
}
END_TEST

START_TEST(test_create_int_tag)
{
    NBT_Tag *intTag = nbtIntTagCreate("test1", 324234);

    ck_assert(intTag ->tagType == TAG_INT);
    ck_assert(intTag->payload.tag_int == 324234);
    ck_assert(intTag->name.length == 5);
    ck_assert(0 == memcmp(intTag->name.payload, "test1", 5));
}
END_TEST

START_TEST(test_create_long_tag)
{
    NBT_Tag *longTag = nbtLongTagCreate("test1", 10000000000000L);

    ck_assert(longTag->tagType == TAG_LONG);
    ck_assert(longTag->payload.tag_long == 10000000000000L);
    ck_assert(longTag->name.length == 5);
    ck_assert(0 == memcmp(longTag->name.payload, "test1", 5));
}
END_TEST

START_TEST(test_create_float_tag)
{
    NBT_Tag *floatTag = nbtFloatTagCreate("test1", 0.5346f);

    ck_assert(floatTag->tagType == TAG_FLOAT);
    ck_assert(floatTag->payload.tag_float == 0.5346f);
    ck_assert(floatTag->name.length == 5);
    ck_assert(0 == memcmp(floatTag->name.payload, "test1", 5));
}
END_TEST

START_TEST(test_create_double_tag)
{
    NBT_Tag *doubleTag = nbtDoubleTagCreate("test1", 0.5346f);

    ck_assert(doubleTag->tagType == TAG_DOUBLE);
    ck_assert(doubleTag->payload.tag_double == 0.5346f);
    ck_assert(doubleTag->name.length == 5);
    ck_assert(0 == memcmp(doubleTag->name.payload, "test1", 5));
}
END_TEST

START_TEST(test_create_bytearray_tag)
{
    NBT_Tag *bytearrayTag = nbtByteArrayTagCreate("test1", 12, "Hello world");

    ck_assert(bytearrayTag->tagType == TAG_BYTE_ARRAY);
    ck_assert(0 == memcmp(bytearrayTag->payload.tag_bytearray.payload, "Hello world", 12));
    ck_assert(bytearrayTag->payload.tag_bytearray.size == 12);
    ck_assert(bytearrayTag->name.length == 5);
    ck_assert(0 == memcmp(bytearrayTag->name.payload, "test1", 5));
}
END_TEST

START_TEST(test_create_string_tag)
{
    NBT_Tag *stringTag = nbtStringTagCreate("test1", "Hello world");

    ck_assert(stringTag->tagType == TAG_STRING);
    ck_assert(0 == memcmp(stringTag->payload.tag_string.payload, "Hello world", 11));
    ck_assert(stringTag->payload.tag_string.length == 11);
    ck_assert(stringTag->name.length == 5);
    ck_assert(0 == memcmp(stringTag->name.payload, "test1", 5));
}
END_TEST

START_TEST(test_create_list_tag)
{
    NBT_Tag *listTag = nbtListTagCreate("test1", TAG_INT, 10);

    ck_assert(listTag->tagType == TAG_LIST);
    ck_assert(listTag->payload.tag_list.length == 10);
    ck_assert(listTag->payload.tag_list.payloadType == TAG_INT);

    ck_assert(listTag->name.length == 5);
    ck_assert(0 == memcmp(listTag->name.payload, "test1", 5));

    NBTPayload payload;
    payload.tag_int = 21;

    nbtListTagAddElement(listTag, payload);

    ck_assert(listTag->payload.tag_list.payload[0].tag_int == 21);

    payload.tag_int = 43;
    nbtListTagAddElement(listTag, payload);

    ck_assert(listTag->payload.tag_list.payload[1].tag_int == 43);
}
END_TEST

START_TEST(test_create_compound_tag)
{
    NBT_Tag *compoundTag = nbtCompoundTagCreate("test1");
    
    ck_assert(compoundTag->tagType == TAG_COMPOUND);
    ck_assert(compoundTag->payload.tag_compound[0].tagType == TAG_END);

    NBT_Tag *intTag = nbtIntTagCreate("test2", 54);
    nbtCompoundTagAddTag(compoundTag, intTag);

    ck_assert(compoundTag->payload.tag_compound[0].payload.tag_int == 54);
    ck_assert(compoundTag->payload.tag_compound[0].tagType == TAG_INT);
    ck_assert(compoundTag->payload.tag_compound[1].tagType == TAG_END);

    ck_assert(compoundTag->name.length == 5);
    ck_assert(0 == memcmp(compoundTag->name.payload, "test1", 5));
}
END_TEST


START_TEST(test_serialize_tag)
{
    Buffer *testBuffer = createBuffer();

    NBT_Tag *compoundTag = nbtCompoundTagCreate("root");
    
    nbtCompoundTagAddTag(compoundTag, nbtIntTagCreate("int", 12345));
    nbtCompoundTagAddTag(compoundTag, nbtStringTagCreate("Name", "This is a test"));
    
    NBT_Tag *listTag = nbtListTagCreate("List", TAG_INT, 3);

    NBTPayload payload;

    payload.tag_int = 1;
    nbtListTagAddElement(listTag, payload);

    payload.tag_int = 2;
    nbtListTagAddElement(listTag, payload);

    payload.tag_int = 3;
    nbtListTagAddElement(listTag, payload);

    nbtCompoundTagAddTag(compoundTag, listTag);
    
    nbtWriteTagInBuffer(compoundTag, testBuffer);

    char data[500];
    readBuffer(testBuffer, data, 500);

    ck_assert(0 == memcmp(data, "\x0a\x00\x04root"
                                "\x03\x00\x03int\x00\x00\x30\x39"
                                "\x08\x00\x04Name\x00\x0eThis is a test"
                                "\x09\x00\x04List\x03\x00\x00\x00\x03\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x03\x00", 65));
}
END_TEST


START_TEST(test_deserialize_tag)
{
    Buffer *serializedTag = createBuffer();

    writeBuffer(serializedTag, "\x0a\x00\x04root"
                                "\x03\x00\x03int\x00\x00\x30\x39"
                                "\x08\x00\x04Name\x00\x0eThis is a test"
                                "\x09\x00\x04List\x03\x00\x00\x00\x03\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x03\x00", 65);

    NBT_Tag *tag = nbtReadTagFromBuffer(serializedTag);
    
    ck_assert(tag->name.length == 4);
    ck_assert(0 == memcmp(tag->name.payload, "root", 4));

    ck_assert(tag->tagType == TAG_COMPOUND);

    ck_assert(tag->payload.tag_compound[0].tagType == TAG_INT);
    ck_assert(tag->payload.tag_compound[0].name.length == 3);
    ck_assert(0 == memcmp(tag->payload.tag_compound[0].name.payload, "int", 3));
    ck_assert(tag->payload.tag_compound[0].payload.tag_int == 12345);

    ck_assert(tag->payload.tag_compound[1].tagType == TAG_STRING);
    ck_assert(tag->payload.tag_compound[1].name.length == 4);
    ck_assert(0 == memcmp(tag->payload.tag_compound[1].name.payload, "Name", 4));
    ck_assert(tag->payload.tag_compound[1].payload.tag_string.length == 14);
    ck_assert(0 == memcmp(tag->payload.tag_compound[1].payload.tag_string.payload, "This is a test", 14));

    NBT_Tag taglist = tag->payload.tag_compound[2];
    ck_assert(taglist.tagType == TAG_LIST);
    ck_assert(taglist.name.length == 4);
    ck_assert(0 == memcmp(taglist.name.payload, "List", 4));
    ck_assert(taglist.payload.tag_list.payloadType == TAG_INT);
    ck_assert(taglist.payload.tag_list.length == 3);
    ck_assert(taglist.payload.tag_list.payload[0].tag_int == 1);
    ck_assert(taglist.payload.tag_list.payload[1].tag_int == 2);
    ck_assert(taglist.payload.tag_list.payload[2].tag_int == 3);

    ck_assert(tag->payload.tag_compound[3].tagType == TAG_END);

}
END_TEST

Suite *nbt_suite()
{
    Suite *suite = suite_create("NBT");

    TCase *tagsCase = tcase_create("Tags");
    tcase_add_test(tagsCase, test_create_end_tag);
    tcase_add_test(tagsCase, test_create_byte_tag);
    tcase_add_test(tagsCase, test_create_short_tag);
    tcase_add_test(tagsCase, test_create_int_tag);
    tcase_add_test(tagsCase, test_create_long_tag);
    tcase_add_test(tagsCase, test_create_float_tag);
    tcase_add_test(tagsCase, test_create_double_tag);
    tcase_add_test(tagsCase, test_create_bytearray_tag);
    tcase_add_test(tagsCase, test_create_string_tag);
    tcase_add_test(tagsCase, test_create_list_tag);
    tcase_add_test(tagsCase, test_create_compound_tag);

    TCase *serializationCase = tcase_create("Serialization");
    tcase_add_test(serializationCase, test_serialize_tag);
    tcase_add_test(serializationCase, test_deserialize_tag);
    
    suite_add_tcase(suite, tagsCase);
    suite_add_tcase(suite, serializationCase);

    return suite;
}
