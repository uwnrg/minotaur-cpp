#ifndef MINOTAUR_SQUARES_H
#define MINOTAUR_SQUARES_H

#include "modify.h"

class Squares : public VideoModifier {
public:
    void modify(cv::UMat &img) override;
};


#endif