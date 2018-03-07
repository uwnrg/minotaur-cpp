#include "recorder.h"

#include "../utility/utility.h"

#include <opencv2/videoio/videoio_c.h>

Recorder::Recorder(QObject *parent, double frame_rate, bool color) :
    QObject(parent),
    m_frame_rate(frame_rate),
    m_color(color),
    m_recording(false) {}

bool Recorder::is_recording() const {
    return m_recording;
}

void Recorder::start_recording(const QString &file, int width, int height) {
    m_video_writer = std::make_unique<cv::VideoWriter>(
        file.toStdString(),
        CV_FOURCC('M', 'J', 'P', 'G'),
        m_frame_rate,
        cv::Size(width, height),
        m_color
    );
    m_recording = true;
}

void Recorder::stop_recording() {
    if (m_video_writer) {
        if (m_video_writer->isOpened()) { m_video_writer->release(); }
        m_video_writer.release();
    }
    m_recording = false;
}

void Recorder::frame_received(const cv::UMat &img) {
    if (m_recording && m_video_writer) {
        m_video_writer->write(img.getMat(cv::ACCESS_READ));
    }
}
