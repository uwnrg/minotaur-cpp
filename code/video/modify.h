#ifndef MINOTAUR_CPP_MODIFY_H
#define MINOTAUR_CPP_MODIFY_H

#include <opencv2/core/mat.hpp>

class VideoModifier {
public:
    virtual void modify(cv::Mat *img) = 0;
};

#endif //MINOTAUR_CPP_MODIFY_H
