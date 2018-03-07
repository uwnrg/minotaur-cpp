#include "preprocessor.h"

#include "../video/modify.h"

#include <opencv2/imgproc.hpp>

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

Preprocessor::Preprocessor(QObject *parent) :
    QObject(parent) {}

void Preprocessor::zoom_changed(double zoom_factor) {
    m_zoom_factor = zoom_factor;
}

void Preprocessor::convert_rgb(bool convert_rgb) {
    m_convert_rgb = convert_rgb;
}

void Preprocessor::preprocess_frame(const cv::UMat &frame) {
    __preprocess_frame(frame);
}

void Preprocessor::__preprocess_frame(cv::UMat frame) {
    // Modifier frame
    if (m_modifier) { m_modifier->modify(frame); }
    // Frame zoom
    zoom(frame, frame, m_zoom_factor);
    // Convert to RGB
    if (m_convert_rgb) { cv::cvtColor(frame, frame, CV_BGR2RGB); }
    // Emit preprocessed frame
    Q_EMIT frame_processed(frame);
}
