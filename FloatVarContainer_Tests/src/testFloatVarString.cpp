#include "pch.h"

using namespace float_var_container_lib;

TEST(CheckFloatVarString, TestConstructorForString) {
    FloatVarString str("What");

    EXPECT_EQ(str()->binary, "What");
    EXPECT_TRUE(true);

    EXPECT_EQ(*(const int*)str()->data_type, *(const int*)str.sType);
    EXPECT_TRUE(true);
}

TEST(CheckFloatVarString, TestConstructorForAdditionalFloatVar) {
    FloatVar fv;
    fv.binary = "No";
    memcpy(fv.data_type, FloatVarString::sType, 4);

    FloatVarString str(fv);

    EXPECT_EQ(str()->binary, "No");
    EXPECT_TRUE(true);

    EXPECT_EQ(*(const int*)str()->data_type, *(const int*)str.sType);
    EXPECT_TRUE(true);
}

TEST(CheckFloatVarString, TestAssignString) {
    FloatVarString str;

    str = "What is that??";

    EXPECT_EQ(str()->binary, "What is that??");
    EXPECT_TRUE(true);

    EXPECT_EQ(*(const int*)str()->data_type, *(const int*)str.sType);
    EXPECT_TRUE(true);
}
