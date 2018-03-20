#include "modify.h"

#include "squares.h"
#include "shapedetect.h"

#ifndef TRACKER_OFF
#include "tracker.h"
#endif

std::shared_ptr<VideoModifier> VideoModifier::get_modifier(int modifier) {
    switch (modifier) {
        case SQUARES:
            return std::make_shared<Squares>();
        case SHAPEDETECT:
            return std::make_shared<ShapeDetect>();
#ifndef TRACKER_OFF
        case OBJTRACK:
            return std::make_shared<TrackerModifier>();
#endif
        default:
            return nullptr;
    }
}

void VideoModifier::add_modifier_list(QComboBox *list) {
    list->addItem("None");
    list->addItem("Square");
    list->addItem("Shape Detector");
#ifndef TRACKER_OFF
    list->addItem("Object Tracker");
#endif
}

void VideoModifier::register_actions(ActionBox *) {}
