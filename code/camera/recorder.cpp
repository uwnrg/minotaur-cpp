#include "recorder.h"

#include "../utility/utility.h"

#include <opencv2/videoio/videoio_c.h>

Recorder::Recorder(int frame_rate, bool color) :
    m_frame_rate(frame_rate),
    m_color(color),
    m_recording(false) {}

bool Recorder::is_recording() const {
    return m_recording;
}

void Recorder::start_recording(const QString &file, int width, int height) {
    // Create the video writer
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
    // If the video writer is active, release its resources
    if (m_video_writer) {
        if (m_video_writer->isOpened()) { m_video_writer->release(); }
        m_video_writer.release();
    }
    m_recording = false;
}

void Recorder::frame_received(const cv::UMat &img) {
    // Write the frame if recording
    if (m_recording && m_video_writer) {
        m_video_writer->write(img.getMat(cv::ACCESS_READ));
    }
}
