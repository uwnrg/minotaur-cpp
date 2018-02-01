/**
 * Detects and tags, rectangles, triangles, and circles.
 * Returns vertices of rectangles and triangles, and radius + center of circles
 *
 * @author Beini Fang
 * @file shapedetect.h
 */
#ifndef MINOTAUR_CPP_SHAPEDETECT_H
#define MINOTAUR_CPP_SHAPEDETECT_H

#include "modify.h"

class Shapedetect : public VideoModifier {
public:
    void modify(cv::Mat *img) override;
};


#endif //MINOTAUR_CPP_SHAPEDETECT_H
