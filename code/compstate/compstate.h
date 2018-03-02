#ifndef MINOTAUR_CPP_COMPSTATE_H
#define MINOTAUR_CPP_COMPSTATE_H

#include <opencv2/core/types.hpp>

#include <QVector2D>

class CompetitionState {
public:
    typedef bool wall_t;

    enum ObjectType {
        CIRCLE,
        TRIANGLE,
        SQUARE,
        UNACQUIRED
    };

    enum {
        wall_is_bool = std::is_same<wall_t, bool>::value,
        wall_is_int = std::is_same<wall_t, int>::value
    };

    /**
     * @return true if the wall array has been initialized
     */
    static bool is_walls_initialized();

    /**
     * Initialize the walls array.
     *
     * @param x the number of columns
     * @param y the number of rows
     * @return false if the array was not already initialized
     */
    static bool reinitialize_walls(int x, int y);

    /**
     * Return the terrain at the given position.
     *
     * @param x the column number
     * @param y the row number
     * @return the terrain value
     */
    static wall_t wall(int x, int y);

    static wall_t **wall_ptr();

    static int object_type();

    static void delete_walls();

    static void assume_ownership

private:
    static bool s_tracking_robot;
    static bool s_tracking_object;
    static bool s_acquire_walls;

    static cv::Rect2d s_robot_box;
    static cv::Rect2d s_object_box;

    static wall_t **s_walls;
    static int s_walls_x;
    static int s_walls_y;

    static int s_object_type;
};

#endif //MINOTAUR_CPP_COMPSTATE_H
