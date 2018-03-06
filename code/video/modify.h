#ifndef MINOTAUR_CPP_MODIFY_H
#define MINOTAUR_CPP_MODIFY_H

#include <memory>

#include <opencv2/core/core.hpp>

#include <QObject>
#include <QComboBox>

#include "../camera/actionbox.h"

class VideoModifier : public QObject {
public:
    enum {
        NONE = 0,
        SQUARES = 1,
        SHAPEDETECT = 2,
        OBJTRACK = 3
    };

    static void attachModifier(std::unique_ptr<VideoModifier> &ptr, int modifier);

    static void addModifierList(QComboBox *list);

    virtual void modify(cv::UMat &img) = 0;

    virtual void forwardKeyEvent(int);

    virtual void register_actions(const std::vector<ActionButton *> &action_btns, ActionBox *box);

    virtual int num_buttons() const;
};

#endif //MINOTAUR_CPP_MODIFY_H
