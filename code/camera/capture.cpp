#include <QBasicTimer>
#include <QTimerEvent>

#include "capture.h"
#include "../utility/utility.h"
#include "../simulator/fakecamera.h"

/**
 * Timer fires at a fixed interval to pull images
 * from the capture.
 */
static QBasicTimer s_capture_timer;

Capture::Capture() = default;

void Capture::start_capture(int cam) {
    // Create the capture instance
    if (cam == FakeCamera::FAKE_CAMERA) {
        // Override capture instance with FakeCamera
        // if it has been selected
        m_video_capture = std::make_unique<FakeCamera>();
    } else {
        m_video_capture = std::make_unique<cv::VideoCapture>(cam);
    }
    if (m_video_capture->isOpened()) {
        // Max at 30 frames per second
        // Limit mainly applies to the FakeCamera so that
        // Qt's event resources are clogged up
        s_capture_timer.start(33, this);
        Q_EMIT capture_started();
    }
}

void Capture::stop_capture() {
    s_capture_timer.stop();
    // Release the video capture resources
    if (m_video_capture && m_video_capture->isOpened()) {
        m_video_capture->release();
    }
    Q_EMIT capture_stopped();
}

void Capture::change_camera(int camera) {
    // Halt current capture and start with the new camera
    stop_capture();
    start_capture(camera);
}

void Capture::timerEvent(QTimerEvent *ev) {
    if (ev->timerId() != s_capture_timer.timerId()) {
        return;
    }
    // Grab the frame from the video capture and emit
    cv::UMat frame;
    *m_video_capture >> frame;
    Q_EMIT frame_ready(frame);
}

int Capture::capture_width() const {
    return static_cast<int>(m_video_capture->get(cv::CAP_PROP_FRAME_WIDTH));
}

int Capture::capture_height() const {
    return static_cast<int>(m_video_capture->get(cv::CAP_PROP_FRAME_HEIGHT));
}
