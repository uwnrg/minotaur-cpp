#ifndef TRACKER_OFF

#include <opencv2/opencv.hpp>
#include <QDialog>
#include <QPushButton>

#include "tracker.h"
#include "../compstate/manager.h"

#ifndef NDEBUG
#include <QDebug>
#endif

// CMake will try to find goturn.caffemodel and goturn.prototxt, which need
// to be added separately. If these are found, the GOTURN tracker model
// will be used instead of the MIL tracker.
#ifdef GOTURN_FOUND
#define TRACKER_TYPE Type::GOTURN
#else
#define TRACKER_TYPE Type::MIL
#endif

TrackerModifier::TrackerModifier(Target target)
    : m_bounding_box(),
      m_type(TRACKER_TYPE),
      m_state(State::UNINITIALIZED),
      m_target(target) {
    reset_tracker();
    connect(this, &TrackerModifier::object_box, &Manager::state(), &CompetitionState::acquire_robot_box);
}

void TrackerModifier::reset_tracker() {
    m_mutex.lock();
    switch (m_type) {
        case Type::BOOSTING:
            m_tracker = cv::TrackerBoosting::create();
            break;
        case Type::MIL:
            m_tracker = cv::TrackerMIL::create();
            break;
        case Type::KCF:
            m_tracker = cv::TrackerKCF::create();
            break;
        case Type::TLD:
            m_tracker = cv::TrackerTLD::create();
            break;
        case Type::MEDIAN_FLOW:
            m_tracker = cv::TrackerMedianFlow::create();
            break;
        case Type::GOTURN:
#ifndef NDEBUG
            qDebug() << "Using GOTURN tracker";
#endif
            m_tracker = cv::TrackerGOTURN::create();
            break;
        default:
            break;
    }
    m_mutex.unlock();
}

void TrackerModifier::beginTracking() {
    if (m_state == State::UNINITIALIZED) {
        m_state = State::FIRST_SCAN;
    }
}

void TrackerModifier::stopTracking() {
    if (m_state != State::UNINITIALIZED) {
        reset_tracker();
        m_state = State::UNINITIALIZED;
        m_bounding_box = {};
    }
}

void TrackerModifier::modify(cv::UMat &img) {
    if (m_state != State::UNINITIALIZED) {
        m_mutex.lock();
        if (m_state == State::TRACKING) {
            m_tracker->update(img, m_bounding_box);
        } else if (m_state == State::FIRST_SCAN) {
            m_bounding_box = cv::selectROI(img);
            m_tracker->init(img, m_bounding_box);
            m_state = State::TRACKING;
        }
        m_mutex.unlock();
        cv::rectangle(img, m_bounding_box.br(), m_bounding_box.tl(), cv::Scalar(255, 0, 0));
        double cx = m_bounding_box.x + m_bounding_box.width / 2.0;
        double cy = m_bounding_box.y + m_bounding_box.height / 2.0;
        if (m_label_btn) m_label_btn->setText(QString::fromStdString("(" + std::to_string(cx) + ", " + std::to_string(cy) + ")"));
        Q_EMIT object_box(m_bounding_box);
    }
}

#include "../gui/mainwindow.h"

void TrackerModifier::start_square_proc() {
    qDebug() << MainWindow::s_sol;
    if (!MainWindow::s_sol) return;
    std::weak_ptr<Solenoid> sol = *MainWindow::s_sol;
    m_procedure = std::unique_ptr<Procedure>(new Procedure(sol, {
        {380, 480},
        {380, 380},
        {480, 380},
        {480, 480},
        {380, 480}
    }));
    m_procedure->start();
}

void TrackerModifier::register_actions(ActionBox *box) {
    ActionButton *start_button = box->add_action("Select ROI");
    ActionButton *clear_button = box->add_action("Clear ROI");
    ActionButton *move_square = box->add_action("Move Square");
    m_label_btn = box->add_action("(0.0, 0.0)");
    connect(start_button, &ActionButton::clicked, this, &TrackerModifier::beginTracking);
    connect(clear_button, &ActionButton::clicked, this, &TrackerModifier::stopTracking);
    connect(move_square, &ActionButton::clicked, this, &TrackerModifier::start_square_proc);
    box->set_actions();
}

#endif