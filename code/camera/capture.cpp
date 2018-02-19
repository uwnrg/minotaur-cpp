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
    cv::Mat frame;
    if (!m_video_capture->read(frame)) {
        m_timer.stop();
        return;
    }
    Q_EMIT matReady(frame);
}
