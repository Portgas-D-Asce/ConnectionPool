#include <gtest/gtest.h>

int add(int x, int y) {
    return x + y;
}

TEST(add, add1) {
    EXPECT_EQ(3, add(1, 2));
}
