//
// Created by Beini Fang on 2018-01-25.
//

#ifndef MINOTAUR_CPP_SHAPEDETECT_H
#define MINOTAUR_CPP_SHAPEDETECT_H

#include "modify.h"

class Shapedetect : public VideoModifier {
public:
    void modify(cv::Mat *img) override;
};


#endif //MINOTAUR_CPP_SHAPEDETECT_H
