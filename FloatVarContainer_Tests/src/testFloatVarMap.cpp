#include "pch.h"

using namespace float_var_container_lib;

TEST(CheckFloatVarMap, TestConstructor) {
    FloatVarMap fvmap;

    EXPECT_EQ(fvmap.size(), 2);
    EXPECT_TRUE(true);

    EXPECT_EQ(*(const int*)fvmap.getFloatVar()->data_type, *(const int*)"map_");
    EXPECT_TRUE(true);

    EXPECT_EQ(*(const int*)fvmap.getKeys().data_type, *(const int*)"mpki");
    EXPECT_TRUE(true);

    EXPECT_EQ(*(const int*)fvmap.getValues().data_type, *(const int*)"mpvl");
    EXPECT_TRUE(true);
}

TEST(CheckFloatVarMap, TestInsertion) {
    FloatVarMap fvmap;

    FloatVar fv1;
    fv1.binary = "what";
    fv1.name = "var";
    memcpy(fv1.data_type, "str ", 5);

    FloatVar fv2;
    fv2.binary = "is";
    fv2.name = "this?";
    memcpy(fv2.data_type, "str ", 5);

    FloatVar fv3;
    fv3.binary = "huh?";
    fv3.name = "you don't";
    memcpy(fv3.data_type, "know", 5);

    FloatVar fv4;
    fv4.setBinaryValue(64);
    fv4.name = "name?";
    memcpy(fv4.data_type, "numb", 5);

    fvmap.insert(fv1, fv2);
    fvmap.insert(fv3, fv4);

    EXPECT_EQ(fvmap.getValue(fv1)->name, fv2.name);
    EXPECT_TRUE(true);

    EXPECT_EQ(fvmap.getValue(fv1)->binary, fv2.binary);
    EXPECT_TRUE(true);

    EXPECT_EQ(*(const int*)fvmap.getValue(fv1)->data_type, *(const int*)fv2.data_type);
    EXPECT_TRUE(true);

    EXPECT_EQ(fvmap.getValue(fv3)->name, fv4.name);
    EXPECT_TRUE(true);

    EXPECT_EQ(fvmap.getValue(fv3)->binary, fv4.binary);
    EXPECT_TRUE(true);

    EXPECT_EQ(*(const int*)fvmap.getValue(fv3)->data_type, *(const int*)fv4.data_type);
    EXPECT_TRUE(true);
}
