#include "modify.h"
#include "squares.h"
#include "shapedetect.h"
#ifndef TRACKER_OFF
#include "tracker.h"
#endif

void VideoModifier::attachModifier(std::unique_ptr<VideoModifier> &ptr, int modifier) {
    switch (modifier) {
        case SQUARES:
            ptr.reset(new Squares);
            break;
        case SHAPEDETECT:
            ptr.reset(new ShapeDetect);
            break;
#ifndef TRACKER_OFF
        case OBJTRACK:
            ptr.reset(new TrackerModifier);
            break;
#endif
        default:
            ptr.release();
            break;
    }
}

void VideoModifier::addModifierList(QComboBox *list) {
    list->addItem("None");
    list->addItem("Square");
    list->addItem("Shape Detector");
#ifndef TRACKER_OFF
    list->addItem("Object Tracker");
#endif
}

void VideoModifier::forwardKeyEvent(int) {}

void VideoModifier::register_actions(const std::vector<ActionButton *> &, ActionBox *) {}

int VideoModifier::num_buttons() const {
    return 0;
}
