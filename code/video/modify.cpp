#include "modify.h"
#include "squares.h"
#include "tracker.h"
#include "shapedetect.h"

void VideoModifier::attachModifier(std::unique_ptr<VideoModifier> &ptr, int modifier) {
    switch (modifier) {
        case SQUARES:
            ptr.reset(new Squares);
            break;
        case SHAPEDETECT:
            ptr.reset(new ShapeDetect);
            break;
        case OBJTRACK:
            ptr.reset(new TrackerModifier);
            break;
        default:
            ptr.release();
            break;
    }
}

void VideoModifier::addModifierList(QComboBox *list) {
    list->addItem("None");
    list->addItem("Square");
    list->addItem("Shape Detector");
    list->addItem("Object Tracker");
}

void VideoModifier::forwardKeyEvent(int) {}

void VideoModifier::register_actions(ActionBox *) {}
