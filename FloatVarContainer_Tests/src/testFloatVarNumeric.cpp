#include "pch.h"

using namespace float_var_container_lib;

TEST(CheckFloatVarNumeric, TestConstructorForFloat) {
    FloatVarNumeric fvn(6.4f);

    EXPECT_EQ(fvn.getValue<float>(), 6.4f);
    EXPECT_TRUE(true);
    EXPECT_EQ(fvn.isFloat(), true);
    EXPECT_TRUE(true);
    EXPECT_EQ(*(const int*)fvn()->data_type, *(const int*)FloatVarNumeric::sTypes[9]);
    EXPECT_TRUE(true);
}

TEST(CheckFloatVarNumeric, TestConstructorForUnsignedLong8) {
    FloatVarNumeric fvn(15935700835712501048);

    EXPECT_EQ(fvn.getValue<unsigned long long>(), 15935700835712501048);
    EXPECT_TRUE(true);
    EXPECT_EQ(fvn.isFloat(), false);
    EXPECT_TRUE(true);
    EXPECT_EQ(*(const int*)fvn()->data_type, *(const int*)FloatVarNumeric::sTypes[8]);
    EXPECT_TRUE(true);
}

TEST(CheckFloatVarNumeric, TestConstructorForChar) {
    FloatVarNumeric fvn('1');

    EXPECT_EQ(fvn.getValue<char>(), '1');
    EXPECT_TRUE(true);
    EXPECT_EQ(fvn.isFloat(), false);
    EXPECT_TRUE(true);
    EXPECT_EQ(*(const int*)fvn()->data_type, *(const int*)FloatVarNumeric::sTypes[1]);
    EXPECT_TRUE(true);
}

TEST(CheckFloatVarNumeric, TestConstructorForDefault) {
    FloatVarNumeric fvn;

    EXPECT_EQ(*(const int*)fvn()->data_type, *(const int*)gDefaultDataType);
    EXPECT_TRUE(true);
}

TEST(CheckFloatVarNumeric, TestAssignForMoreTypes) {
    FloatVarNumeric fvn;
    fvn = 22;
    EXPECT_EQ(fvn.getValue<int>(), 22);
    EXPECT_TRUE(true);
    EXPECT_EQ(*(const int*)fvn()->data_type, *(const int*)fvn.sTypes[3]);
    EXPECT_TRUE(true);

    fvn = 5.3f;
    EXPECT_EQ(fvn.getValue<float>(), 5.3f);
    EXPECT_TRUE(true);
    EXPECT_EQ(*(const int*)fvn()->data_type, *(const int*)fvn.sTypes[9]);
    EXPECT_TRUE(true);
}
