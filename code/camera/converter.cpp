#include "converter.h"
#include "cameradisplay.h"
#include "../utility/utility.h"

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

void Converter::processFrame(const cv::UMat &frame) {
    if (m_process_all) {
        process(frame);
    } else {
        queue(frame);
    }
}

void Converter::modifierChanged(int modifier_index, ActionBox *box) {
    VideoModifier::attachModifier(m_modifier, modifier_index);
    QMetaObject::invokeMethod(box, "reset_actions");
    if (m_modifier) {
        connect(m_display, &CameraDisplay::returnActionButtons, m_modifier.get(), &VideoModifier::register_actions);
        QMetaObject::invokeMethod(m_display, "requestActionButtons", Q_ARG(int, m_modifier->num_buttons()));
    }
}


void Converter::matDelete(void *mat) {
    delete static_cast<cv::UMat *>(mat);
}

void Converter::queue(const cv::UMat &frame) {
    m_frame = frame;
    if (!m_timer.isActive()) {
        m_timer.start(0, this);
    }
}

void Converter::zoom(cv::UMat &frame, cv::UMat &zoomed_frame, double zoom_factor) {
    cv::Rect crop(frame.size().width / 2.0 - frame.size().width / (2.0 * zoom_factor), frame.size().height / 2.0 - frame.size().height / (2.0 * zoom_factor), frame.size().width / zoom_factor, frame.size().height / zoom_factor);
    frame = frame(crop);
    cv::resize(frame, zoomed_frame, cv::Size(), zoom_factor, zoom_factor, cv::INTER_LINEAR);
}

void Converter::process(cv::UMat frame) {
    double scale = MIN(
        (m_display->width() - 20) / (double) frame.size().width,
        (m_display->height() - 20) / (double) frame.size().height
    );
    if (m_modifier) {
        m_modifier->modify(frame);
    }
    if (is_recording()) {
        Q_EMIT frameProcessed(frame);
    }
    cv::UMat zoomed_frame;
    zoom(frame, zoomed_frame, m_zoom);
    cv::resize(zoomed_frame, frame, cv::Size(), scale, scale, cv::INTER_AREA);
    cv::cvtColor(frame, frame, CV_BGR2RGB);
    const QImage image(
        frame.getMat(1).data, frame.cols, frame.rows, static_cast<int>(frame.step),
        QImage::Format_RGB888, &matDelete, new cv::UMat(frame)
    );
#ifndef NDEBUG
    Q_ASSERT(image.constBits() == frame.getMat(1).data);
#endif
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
    ++m_frame_count;
    process(m_frame);
    m_frame.release();
    m_timer.stop();
}

void Converter::startRecording(QString file, int width, int height) {
    constexpr int fps = 30;
    m_recorder = std::make_unique<Recorder>(
        file.toStdString(), CV_FOURCC('M', 'J', 'P', 'G'),
        fps, cv::Size(width, height), true
    );
    connect(this, &Converter::frameProcessed, m_recorder.get(), &Recorder::image_received);
}

void Converter::stopRecording() {
    if (m_recorder) {
        if (m_recorder->is_recording()) {
            m_recorder->stop_recording();
        }
        m_recorder.reset();
    }
}

bool Converter::is_recording() const {
    return m_recorder && m_recorder->is_recording();
}

int Converter::get_and_reset_frames() {
    int frames = m_frame_count;
    m_frame_count = 0;
    return frames;
}

void Converter::set_zoom(double zoom_factor) {
    m_zoom = zoom_factor;
}