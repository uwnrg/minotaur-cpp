#ifndef MINOTAUR_CPP_COMMON_H
#define MINOTAUR_CPP_COMMON_H

#include <vector>

namespace cv {
    template<typename _Tp> class Rect_;
    typedef Rect_<double> Rect2d;
}
namespace nrg {
    template<typename val_t> class vector;
    template<typename val_t> class rect;
}
typedef nrg::vector<double> vector2d;
typedef nrg::rect<double> rect2d;
typedef std::vector<nrg::vector<double>> path2d;

namespace algo {

    vector2d rect_center(const cv::Rect2d &rect);

    /**
     * Returns the intersection point of a line perpendicular to the
     * line passing through points p0 and p1 and that passes through
     * point c.
     *
     * Used to determine perpendicular distance and correction for
     * line straddling.
     *
     * @param c  robot or object location
     * @param p0 start location
     * @param p1 target location
     * @return
     */
    vector2d perp_intersect(
        const vector2d &c,
        const vector2d &p0,
        const vector2d &p1
    );

    /**
     * Generate the eight rectangular traverse points around a
     * rectangle. This is used to generate the 2D graph to
     * move the robot around the object.
     *
     * @param rob robot rectangle
     * @param obj object rectangle
     * @return points clockwise starting with top-left
     */
    path2d object_traverse_points(
        const rect2d &rob,
        const rect2d &obj
    );

    /**
     * Returns the path, including the robot's starting position, to the
     * desired side of the object without colliding with the object.
     *
     * @param rob
     * @param obj
     * @return
     */
    path2d robot_object_path(
        const rect2d &rob,
        const rect2d &obj,
        int side
    );

}

#endif //MINOTAUR_CPP_COMMON_H
