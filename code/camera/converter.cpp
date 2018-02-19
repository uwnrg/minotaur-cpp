#include "converter.h"
#include "cameradisplay.h"

#include <QTimerEvent>

#ifndef NDEBUG
#include <QDebug>
#endif

Converter::Converter(QObject *parent, CameraDisplay *display)
    : QObject(parent),
      m_display(display) {}

void Converter::setProcessAll(bool process_all) {
    m_process_all = process_all;
}

void Converter::processFrame(const cv::Mat &frame) {
    if (m_process_all) {
        process(frame);
    } else {
        queue(frame);
    }
}

void Converter::modifierChanged(int modifier_index) {
    VideoModifier::attachModifier(m_modifier, modifier_index);
}

void Converter::matDelete(void *mat) {
    delete static_cast<cv::Mat *>(mat);
}

void Converter::queue(const cv::Mat &frame) {
    m_frame = frame;
    if (!m_timer.isActive()) {
        m_timer.start(0, this);
    }
}

void Converter::process(cv::Mat frame) {
    double scale = MIN(
        (m_display->width() - 20) / (double) frame.size().width,
        (m_display->height() - 20) / (double) frame.size().height
    );
    if (m_modifier) {
        m_modifier->modify(frame);
    }
    cv::resize(frame, frame, cv::Size(), scale, scale, cv::INTER_AREA);
    cv::cvtColor(frame, frame, CV_BGR2RGB);
    const QImage image(
        frame.data, frame.cols, frame.rows, static_cast<int>(frame.step),
        QImage::Format_RGB888, &matDelete, new cv::Mat(frame)
    );
    Q_ASSERT(image.constBits() == frame.data);
    Q_EMIT imageReady(image);
}

void Converter::imageKeyEvent(int key) {
#ifndef NDEBUG
    qDebug() << "Key pressed: " << key;
#endif
    if (m_modifier) {
        m_modifier->forwardKeyEvent(key);
    }
}

void Converter::timerEvent(QTimerEvent *ev) {
    if (ev->timerId() != m_timer.timerId()) {
        return;
    }
    process(m_frame);
    m_frame.release();
    m_timer.stop();
}
