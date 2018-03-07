#include <gtest/gtest.h>

#include <code/controller/solenoid.h>

TEST(encode_movement, direction_and_time) {
    Vector2i vec(-1234, 1000);
    int time = 240;
    QByteArray result = Solenoid::encode_message(vec, time);
    ASSERT_EQ(result.size(), 5);
    // 1111 1011 0010 1110 = -1234
    // 0000 0011 1110 1000 =  1000
    //           1111 0000 =  240
    ASSERT_EQ(result.at(0), 46);
    ASSERT_EQ(result.at(1), static_cast<char>(251));
    ASSERT_EQ(result.at(2), static_cast<char>(232));
    ASSERT_EQ(result.at(3), 3);
    ASSERT_EQ(result.at(4), static_cast<char>(240));
}
