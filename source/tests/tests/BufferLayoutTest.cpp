#include <gtest/gtest.h>
#include <Hazy/Renderer/Buffer.h>

TEST(BufferLayoutTest, Test1) {
    Hazy::VertexBufferLayout layout;
    EXPECT_EQ(layout.getStride(), 0);
}

TEST(BufferLayoutTest, Test2) {
    Hazy::VertexBufferLayout layout = {
        {Hazy::DataType::Float, 3, "aPosition"},    // 12 bytes
        {Hazy::DataType::Float, 4, "aColor"},       // 16 bytes
        {Hazy::DataType::Float, 2, "aUV"}           // 8 bytes
    };                                              // 36
    EXPECT_EQ(layout.getStride(), 36);
    EXPECT_EQ(layout.getElementCount(), 3);
}

TEST(BufferLayoutTest, Test3) {
    Hazy::VertexBufferLayout layout = {
        {Hazy::DataType::Float, 3, "aPosition"},    // 12 bytes
        {Hazy::DataType::Int, 4, "aColor"},         // 16 bytes
        {Hazy::DataType::Float, 2, "aUV"},          // 8 bytes
        {Hazy::DataType::Bool, 1, "aBool"}          // 1 byte
    };                                              // 37
    EXPECT_EQ(layout.getStride(), 37);
    EXPECT_EQ(layout.getElementCount(), 4);
    
    EXPECT_EQ(layout.getElements()[0].name, "aPosition");
    EXPECT_EQ(layout.getElements()[0].type, Hazy::DataType::Float);
    EXPECT_EQ(layout.getElements()[0].offset, 0);
    EXPECT_EQ(layout.getElements()[0].count, 3);

    EXPECT_EQ(layout.getElements()[1].name, "aColor");
    EXPECT_EQ(layout.getElements()[1].type, Hazy::DataType::Int);
    EXPECT_EQ(layout.getElements()[1].offset, 12);
    EXPECT_EQ(layout.getElements()[1].count, 4);

    EXPECT_EQ(layout.getElements()[2].name, "aUV");
    EXPECT_EQ(layout.getElements()[2].type, Hazy::DataType::Float);
    EXPECT_EQ(layout.getElements()[2].offset, 28);
    EXPECT_EQ(layout.getElements()[2].count, 2);

    EXPECT_EQ(layout.getElements()[3].name, "aBool");
    EXPECT_EQ(layout.getElements()[3].type, Hazy::DataType::Bool);
    EXPECT_EQ(layout.getElements()[3].offset, 36);
    EXPECT_EQ(layout.getElements()[3].count, 1);
}