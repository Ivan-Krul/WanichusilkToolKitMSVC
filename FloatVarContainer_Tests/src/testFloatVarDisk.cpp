#include "pch.h"

using namespace float_var_container_lib;

TEST(CheckFloatVarDisk, TestSave) {
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
    
    FloatVarDisk fvd = fvol;
    fvd.writeFile("file.fvc");

    std::ifstream fin("file.fvc");

    EXPECT_EQ(fin.is_open(), true);
    EXPECT_TRUE(true);

    fin.seekg(0, std::ios::end);
    EXPECT_EQ(fin.tellg(), 82);
    EXPECT_TRUE(true);

    fin.close();
}

TEST(CheckFloatVarDisk, TestLoad) {
    FloatVarDisk fvd;
    auto res = fvd.readFile("file.fvc");

    EXPECT_EQ(res, FloatVarDisk::InteractStatus::success);
    EXPECT_TRUE(true);

    EXPECT_EQ(fvd()->name, "araaau");
    EXPECT_TRUE(true);

    EXPECT_EQ(*(const int*)fvd()->data_type, *(const int*)"objt");
    EXPECT_TRUE(true);

    EXPECT_EQ(fvd()->list[0].binary, "what");
    EXPECT_TRUE(true);

    EXPECT_EQ(fvd()->list[0].name, "var");
    EXPECT_TRUE(true);

    EXPECT_EQ(*(const int*)fvd()->list[0].data_type, *(const int*)"str ");
    EXPECT_TRUE(true);

    EXPECT_EQ(fvd()->list[1].binary, "is");
    EXPECT_TRUE(true);

    EXPECT_EQ(fvd()->list[1].name, "this?");
    EXPECT_TRUE(true);

    EXPECT_EQ(*(const int*)fvd()->list[1].data_type, *(const int*)"str ");
    EXPECT_TRUE(true);

    EXPECT_EQ(fvd()->list[2].binary, "huh?");
    EXPECT_TRUE(true);

    EXPECT_EQ(fvd()->list[2].name, "you don't");
    EXPECT_TRUE(true);

    EXPECT_EQ(*(const int*)fvd()->list[2].data_type, *(const int*)"know");
    EXPECT_TRUE(true);

#ifdef _WIN32
    system("del file.fvc");
#else
    system("rm file.fvc");
#endif
}
