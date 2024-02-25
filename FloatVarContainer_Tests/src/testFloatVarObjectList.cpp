#include "pch.h"

using namespace float_var_container_lib;

TEST(CheckFloatVarObjectList, TestConstructorInitializerBasic) {
    FloatVar fv1;
    fv1.binary = "what";
    fv1.name = "var";
    memcpy(fv1.data_type, "str ", 5);

    FloatVar fv2;
    fv2.binary = "is";
    fv2.name = "this?";
    memcpy(fv2.data_type, "str ", 5);

    FloatVarObjectList fvol = { fv1, fv2 };

    EXPECT_EQ(fvol.size(), 2);
    EXPECT_TRUE(true);

    EXPECT_EQ(fvol.name_valid("var"), true);
    EXPECT_TRUE(true);

    EXPECT_EQ(fvol.at("var").binary, "what");
    EXPECT_TRUE(true);

    EXPECT_EQ(*(const int*)fvol.at("var").data_type, *(const int*)"str ");
    EXPECT_TRUE(true);

    EXPECT_EQ(fvol.name_valid("this?"), true);
    EXPECT_TRUE(true);

    EXPECT_EQ(fvol.at("this?").binary, "is");
    EXPECT_TRUE(true);

    EXPECT_EQ(*(const int*)fvol.at("this?").data_type, *(const int*)"str ");
    EXPECT_TRUE(true);
}

TEST(CheckFloatVarObjectList, TestConstructorInitializerExpander) {
    FloatVarString fv1("String", "is");

    FloatVarNumeric fv2("sized", 2);


    FloatVarObjectList fvol = { fv1, fv2 };

    EXPECT_EQ(fvol.size(), 2);
    EXPECT_TRUE(true);

    EXPECT_EQ(fvol.name_valid("String"), true);
    EXPECT_TRUE(true);

    EXPECT_EQ(fvol.at("String").binary, "is");
    EXPECT_TRUE(true);

    EXPECT_EQ(*(const int*)fvol.at("String").data_type, *(const int*)FloatVarString::sType);
    EXPECT_TRUE(true);

    EXPECT_EQ(fvol.name_valid("sized"), true);
    EXPECT_TRUE(true);

    EXPECT_EQ(fvol.at("sized").convertTo<int>(), 2);
    EXPECT_TRUE(true);

    EXPECT_EQ(*(const int*)fvol.at("sized").data_type, *(const int*)(FloatVarNumeric::sTypes[3]));
    EXPECT_TRUE(true);
}

TEST(CheckFloatVarObjectList, TestConstructorInitializerVector) {
    FloatVar fv1;
    fv1.binary = "what";
    fv1.name = "var";
    memcpy(fv1.data_type, "str ", 5);

    FloatVar fv2;
    fv2.binary = "is";
    fv2.name = "this?";
    memcpy(fv2.data_type, "str ", 5);

    std::vector<FloatVar> vec = { fv1, fv2 };

    FloatVarObjectList fvol = vec;

    EXPECT_EQ(fvol.size(), 2);
    EXPECT_TRUE(true);

    EXPECT_EQ(fvol.name_valid("var"), true);
    EXPECT_TRUE(true);

    EXPECT_EQ(fvol.at("var").binary, "what");
    EXPECT_TRUE(true);

    EXPECT_EQ(*(const int*)fvol.at("var").data_type, *(const int*)"str ");
    EXPECT_TRUE(true);

    EXPECT_EQ(fvol.name_valid("this?"), true);
    EXPECT_TRUE(true);

    EXPECT_EQ(fvol.at("this?").binary, "is");
    EXPECT_TRUE(true);

    EXPECT_EQ(*(const int*)fvol.at("this?").data_type, *(const int*)"str ");
    EXPECT_TRUE(true);
}

TEST(CheckFloatVarObjectList, TestConstructorInitializerVectorExpander) {
    FloatVarString fv1("String", "is");

    FloatVarNumeric fv2("sized", 2);

    FloatVarObjectList fvol;
    fvol.push_back(*fv1.getFloatVar());
    fvol.push_back(*fv2.getFloatVar());

    EXPECT_EQ(fvol.size(), 2);
    EXPECT_TRUE(true);

    EXPECT_EQ(fvol.name_valid("String"), true);
    EXPECT_TRUE(true);

    EXPECT_EQ(fvol.at("String").binary, "is");
    EXPECT_TRUE(true);

    EXPECT_EQ(*(const int*)fvol.at("String").data_type, *(const int*)FloatVarString::sType);
    EXPECT_TRUE(true);

    EXPECT_EQ(fvol.name_valid("sized"), true);
    EXPECT_TRUE(true);

    EXPECT_EQ(fvol.at("sized").convertTo<int>(), 2);
    EXPECT_TRUE(true);

    EXPECT_EQ(*(const int*)fvol.at("sized").data_type, *(const int*)(FloatVarNumeric::sTypes[3]));
    EXPECT_TRUE(true);
}

TEST(CheckFloatVarObjectList, TestBasicObjectManipulation) {
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

    FloatVarObjectList fvol = { fv1, fv2, fv3 };

    FloatVar fv4;
    fv4.setBinaryValue(64);
    fv4.name = "name?";
    memcpy(fv4.data_type, "numb", 5);

    fvol.push_back(fv4);

    EXPECT_EQ(fvol.size(), 4);
    EXPECT_TRUE(true);

    fvol.erase(fvol.begin());
    EXPECT_EQ(fvol.name_valid("var"), false);
    EXPECT_TRUE(true);

    fvol.erase("this");
    EXPECT_EQ(fvol.name_valid("this?"), true);
    EXPECT_TRUE(true);

    fvol.erase("this?");
    EXPECT_EQ(fvol.name_valid("this?"), false);
    EXPECT_TRUE(true);

    EXPECT_EQ(fvol.size(), 2);
    EXPECT_TRUE(true);

    fvol.clear();
    EXPECT_EQ(fvol.size(), 0);
    EXPECT_TRUE(true);
}
