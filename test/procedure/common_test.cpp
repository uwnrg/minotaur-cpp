#include <gtest/gtest.h>
#include <code/compstate/common.h>
#include <code/utility/rect.h>

TEST(common, robot_object_path) {
    rect2d obj(-10, -10, 20, 10);
    rect2d rob(3, 10, 4, 4);
    path2d res_left = algo::robot_object_path(rob, obj, nrg::dir::LEFT);
    path2d exp_left = {
        {5, 12},
        {-12, 2},
        {-12, -5}
    };
    ASSERT_EQ(3, res_left.size());
    for (std::size_t i = 0; i < 3; ++i) {
        ASSERT_EQ(exp_left[i].x(), res_left[i].x());
        ASSERT_EQ(exp_left[i].y(), res_left[i].y());
    }
    path2d res_top = algo::robot_object_path(rob, obj, nrg::dir::TOP);
    path2d exp_top = {
        {5, 12},
        {12, 2},
        {12, -5},
        {12, -12},
        {0, -12}
    };
    ASSERT_EQ(5, res_top.size());
    for (std::size_t i = 0; i < 5; ++i) {
        ASSERT_EQ(exp_top[i].x(), res_top[i].x());
        ASSERT_EQ(exp_top[i].y(), res_top[i].y());
    }
}
