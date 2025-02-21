#include <iostream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(RunTest, DefaultLoop) {
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}
