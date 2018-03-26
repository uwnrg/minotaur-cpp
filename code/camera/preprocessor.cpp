#include "preprocessor.h"

#include "../video/modify.h"

#include <opencv2/imgproc.hpp>
#include <QTimerEvent>

void zoom(cv::UMat &src, cv::UMat &dest, double zoom_factor) {
    cv::Rect crop(
        static_cast<int>(src.size().width / 2.0 - src.size().width / (2.0 * zoom_factor)),
        static_cast<int>(src.size().height / 2.0 - src.size().height / (2.0 * zoom_factor)),
        static_cast<int>(src.size().width / zoom_factor),
        static_cast<int>(src.size().height / zoom_factor)
    );
    src = src(crop);
    cv::resize(src, dest, cv::Size(), zoom_factor, zoom_factor, cv::INTER_LINEAR);
}

void rotate(cv::UMat& src, double angle, cv::UMat& dst) {
    cv::Point2f pt_cp(src.cols * 0.5, src.rows * 0.5);
    cv::Mat mat = cv::getRotationMatrix2D(pt_cp, angle, 1.0);
    cv::warpAffine(src, dst, mat, src.size(), cv::INTER_CUBIC);
}

Preprocessor::Preprocessor() = default;

void Preprocessor::zoom_changed(double zoom_factor) {
    m_zoom_factor = zoom_factor;
}

void Preprocessor::rotation_changed(int angle) {
    m_rotation_angle = angle;
}

void Preprocessor::convert_rgb(bool convert_rgb) {
    m_convert_rgb = convert_rgb;
}

void Preprocessor::use_modifier(const std::shared_ptr<VideoModifier> &modifier) {
    m_modifier = modifier;
}

void Preprocessor::preprocess_frame(const cv::UMat &frame) {
    if (m_process_all) { __preprocess_frame(frame); }
    else { __queue(frame); }
}

void Preprocessor::__preprocess_frame(cv::UMat frame) {
    // Modifier frame
    if (m_modifier) { m_modifier->modify(frame); }
    // Rotate frame
    double angle = static_cast<double>(m_rotation_angle);
    rotate(frame, angle, frame);
    // Frame zoom
    zoom(frame, frame, m_zoom_factor);
    // Convert to RGB
    if (m_convert_rgb) { cv::cvtColor(frame, frame, CV_BGR2RGB); }
    // Emit preprocessed frame
    Q_EMIT frame_processed(frame);
}

void Preprocessor::__queue(const cv::UMat &frame) {
    m_frame = frame;
    if (!m_queue_timer.isActive()) { m_queue_timer.start(0, this); }
}

void Preprocessor::timerEvent(QTimerEvent *ev) {
    if (ev->timerId() != m_queue_timer.timerId()) { return; }
    __preprocess_frame(m_frame);
    m_frame.release();
    m_queue_timer.stop();
}

double Preprocessor::get_zoom_factor() const {
    return m_zoom_factor;
}
