#include "pch.h"

using namespace float_var_container_lib;

TEST(CheckFloatVar, TestForSetInt) {
    FloatVar fv;
    fv.setBinaryValue(15);

    int a = 15;

    EXPECT_EQ(*(int*)fv.binary.data(), a);
    EXPECT_TRUE(true);
}

TEST(CheckFloatVar, TestForSetDouble) {
    FloatVar fv;
    fv.setBinaryValue(0.1);

    double d = 0.1;

    EXPECT_EQ(*(double*)fv.binary.data(), d);
    EXPECT_TRUE(true);
}

TEST(CheckFloatVar, TestForGetInt) {
    FloatVar fv;
    fv.setBinaryValue(50); // trust him

    EXPECT_EQ(fv.convertTo<int>(), 50);
    EXPECT_TRUE(true);
}

TEST(CheckFloatVar, TestForGetFloat) {
    FloatVar fv;
    fv.setBinaryValue(5.1f); // trust him

    EXPECT_EQ(fv.convertTo<float>(), 5.1f);
    EXPECT_TRUE(true);
}
