#include <gtest/gtest.h>

TEST(FooTest, Returns42) {
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
