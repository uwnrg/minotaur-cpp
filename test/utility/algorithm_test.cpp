#include <gtest/gtest.h>
#include <code/utility/algorithm.h>

TEST(algorithm, swept_collide) {
    rect2d ob(0, 0, 2, 2);
    rect2d r0(1, 3, 2, 2);
    rect2d r1_false(4.1, 0, 2, 2);
    rect2d r1_true(3.7, 0, 2, 2);
    ASSERT_FALSE(algo::swept_collide(r0, r1_false, ob));
    ASSERT_TRUE(algo::swept_collide(r0, r1_true, ob));
}
