#include <gtest/gtest.h>

#include <code/utility/utility.h>

TEST(shrink_into, signed_values_min) {
    int16_t expected = std::numeric_limits<int16_t>::min();
    int min_val = std::numeric_limits<int>::min();
    int16_t result = shrink_into<int16_t>()(min_val);
    ASSERT_EQ(expected, result);
}

TEST(shrink_into, signed_values_max) {
    int16_t expected = std::numeric_limits<int16_t>::max();
    int max_val = std::numeric_limits<int>::max();
    int16_t result = shrink_into<int16_t>()(max_val);
    ASSERT_EQ(expected, result);
}

TEST(shrink_into, signed_values_in_range) {
    double value = 123.2;
    int16_t expected = 123;
    int16_t result = shrink_into<int16_t>()(value);
    ASSERT_EQ(expected, result);
}
