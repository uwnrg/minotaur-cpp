#include "preprocessor.h"

#include "../video/modify.h"

#include <opencv2/imgproc.hpp>
#include <QTimerEvent>

void zoom(cv::UMat &src, cv::UMat &dest, double zoom_factor) {
    // Crop the image to the zoom area
    cv::Rect crop(
        static_cast<int>(src.size().width / 2.0 - src.size().width / (2.0 * zoom_factor)),
        static_cast<int>(src.size().height / 2.0 - src.size().height / (2.0 * zoom_factor)),
        static_cast<int>(src.size().width / zoom_factor),
        static_cast<int>(src.size().height / zoom_factor)
    );
    src = src(crop);
    // Resize the image up to the original size
    cv::resize(src, dest, cv::Size(), zoom_factor, zoom_factor, cv::INTER_LINEAR);
}

void rotate(cv::UMat& src, double angle, cv::UMat& dst) {
    // Rotate the image
    cv::Point2f pt_cp(src.cols * 0.5f, src.rows * 0.5f);
    cv::Mat mat = cv::getRotationMatrix2D(pt_cp, angle, 1.0);
    cv::warpAffine(src, dst, mat, src.size(), cv::INTER_CUBIC);
}

Preprocessor::Preprocessor() :
    m_zoom_factor(1.0),
    m_rotation_angle(0),
    m_convert_rgb(true),
    m_process_all(false) {}

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
    // Queue the frame unless all frames are to be processed
    if (m_process_all) { preprocess_frame_delegate(frame); }
    else { queue_frame(frame); }
}

void Preprocessor::preprocess_frame_delegate(cv::UMat frame) {
    // Modifier frame
    if (m_modifier) { m_modifier->modify(frame); }
    // Rotate frame
    rotate(frame, static_cast<double>(m_rotation_angle), frame);
    // Frame zoom
    zoom(frame, frame, m_zoom_factor);
    // Convert to RGB
    if (m_convert_rgb) { cv::cvtColor(frame, frame, CV_BGR2RGB); }
    // Emit preprocessed frame
    Q_EMIT frame_processed(frame);
}

void Preprocessor::queue_frame(const cv::UMat &frame) {
    // Load the frame into the single-element queue
    m_frame = frame;
    // If there is no current frame being processed, start the timer
    if (!m_queue_timer.isActive()) { m_queue_timer.start(0, this); }
}

void Preprocessor::timerEvent(QTimerEvent *ev) {
    if (ev->timerId() != m_queue_timer.timerId()) { return; }
    // Process the frame; this function is blocking and will not
    // return until the frame is processed, and usually has slower
    // execution time, which limits frame rate
    preprocess_frame_delegate(m_frame); // pass a copy of the pointer
    m_frame.release();
    // Stop the timer so that the next received frame can be processed
    m_queue_timer.stop();
}

double Preprocessor::get_zoom_factor() const {
    return m_zoom_factor;
}
