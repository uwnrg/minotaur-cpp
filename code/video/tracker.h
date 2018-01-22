#ifndef MINOTAUR_CPP_TRACKER_H
#define MINOTAUR_CPP_TRACKER_H

#include "modify.h"

class TrackerModifier : public VideoModifier {
public:
    void modify(cv::Mat *img) override;

private:
};

#endif //MINOTAUR_CPP_TRACKER_H
