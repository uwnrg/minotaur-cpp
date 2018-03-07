#include "capture.h"

#include "../utility/utility.h"

#include <QTimerEvent>

Capture::Capture() = default;

void Capture::start_capture(int cam) {
    if (cam < 0) { return; }
    m_video_capture = std::make_unique<cv::VideoCapture>(cam);
    if (m_video_capture->isOpened()) {
        m_timer.start(0, this);
        Q_EMIT capture_started();
    }
}

void Capture::stop_capture() {
    m_timer.stop();
    m_video_capture->release();
    Q_EMIT capture_stopped();
}

void Capture::timerEvent(QTimerEvent *ev) {
    if (ev->timerId() != m_timer.timerId()) {
        return;
    }
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
