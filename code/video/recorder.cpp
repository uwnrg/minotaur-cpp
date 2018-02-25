#include "recorder.h"

#ifndef NDEBUG
#include <QDebug>
#endif

Recorder::Recorder(
        const std::string &file_name,
        int four_cc,
        double frame_rate,
        cv::Size size,
        bool is_color
) : m_video_writer(file_name, four_cc, frame_rate, size, is_color),
    m_is_recording(true) {}

void Recorder::modify(cv::Mat *img) {
    m_video_writer.write(*img);
}

void Recorder::finish() {
    if (m_video_writer.isOpened()) {
        m_video_writer.release();
    }
#ifndef NDEBUG
    else {
        qDebug() << "Attempted to release video writer that is not open";
    }
#endif
    m_is_recording = false;
}

bool Recorder::is_recording() {
    return m_is_recording;
}

void Recorder::image_received(cv::Mat &img) {
    modify(&img);
}

void Recorder::stop_recording() {
    finish();
}