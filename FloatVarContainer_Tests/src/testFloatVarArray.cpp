#include "pch.h"

using namespace float_var_container_lib;

TEST(CheckFloatVarArray, TestAccessToElement) {
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

    FloatVarArray fvol = { fv1, fv2, fv3 };
    fvol()->name = "araaau";

    EXPECT_EQ(fvol[0].binary, "what");
    EXPECT_TRUE(true);

    EXPECT_EQ(fvol[0].name, "var");
    EXPECT_TRUE(true);

    EXPECT_EQ(*(const int*)fvol[0].data_type, *(const int*)"str ");
    EXPECT_TRUE(true);

    EXPECT_EQ(fvol[1].binary, "is");
    EXPECT_TRUE(true);

    EXPECT_EQ(fvol[1].name, "this?");
    EXPECT_TRUE(true);

    EXPECT_EQ(*(const int*)fvol[1].data_type, *(const int*)"str ");
    EXPECT_TRUE(true);

    EXPECT_EQ(fvol[2].binary, "huh?");
    EXPECT_TRUE(true);

    EXPECT_EQ(fvol[2].name, "you don't");
    EXPECT_TRUE(true);

    EXPECT_EQ(*(const int*)fvol[2].data_type, *(const int*)"know");
    EXPECT_TRUE(true);
}
