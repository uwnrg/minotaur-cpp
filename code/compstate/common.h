#ifndef MINOTAUR_CPP_COMMON_H
#define MINOTAUR_CPP_COMMON_H

#include "../utility/vector.h"

namespace algo {

    vector2d rect_center(const cv::Rect2d &rect);

    vector2d perp_intersect(
        const vector2d &c,
        const vector2d &p0,
        const vector2d &p1
    );

}

#endif //MINOTAUR_CPP_COMMON_H
