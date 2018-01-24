#ifndef MINOTAUR_CPP_MODIFY_H
#define MINOTAUR_CPP_MODIFY_H

#include <memory>

#include <opencv2/core/core.hpp>

#include <QComboBox>

class VideoModifier {
public:
    enum {
        NONE,
        SQUARES
    };

    static void attachModifier(std::unique_ptr<VideoModifier> &ptr, int modifier);

    static void addModifierList(QComboBox *list);

    virtual void modify(cv::Mat *img) = 0;
};

#endif //MINOTAUR_CPP_MODIFY_H
