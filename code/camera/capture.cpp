#include "capture.h"

#include <QTimerEvent>

Capture::Capture(QObject *parent)
    : QObject(parent) {}

void Capture::start(int cam) {
    if (cam < 0) {
        return;
    }
    if (!m_video_capture || !m_video_capture->isOpened()) {
        m_video_capture.reset(new cv::VideoCapture(cam));
    }
    if (m_video_capture->isOpened()) {
        m_timer.start(0, this);
        Q_EMIT started();
    }
}

void Capture::stop() {
    m_timer.stop();
    m_video_capture->release();
}

void Capture::timerEvent(QTimerEvent *ev) {
    if (ev->timerId() != m_timer.timerId()) {
        return;
    }
    cv::UMat frame;
    *m_video_capture >> frame;
    Q_EMIT matReady(frame);
}

int Capture::capture_width() const {
    return static_cast<int>(m_video_capture->get(cv::CAP_PROP_FRAME_WIDTH));
}

int Capture::capture_height() const {
    return static_cast<int>(m_video_capture->get(cv::CAP_PROP_FRAME_HEIGHT));
}
