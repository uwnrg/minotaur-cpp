#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <QBasicTimer>
#include <QTimerEvent>

#include "preprocessor.h"
#include "../video/modify.h"

// Static timer instance for queue filling
static QBasicTimer s_queue_timer;
// Static instance of queue'd frame
static cv::UMat s_frame;

static void zoom(cv::UMat &src, cv::UMat &dest, double zoom_factor) {
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

static void rotate(cv::UMat& src, double angle, cv::UMat& dst) {
    // Rotate the image
    cv::Point2f pt_cp(src.cols * 0.5f, src.rows * 0.5f);
    cv::Mat mat = cv::getRotationMatrix2D(pt_cp, angle, 1.0);
    cv::warpAffine(src, dst, mat, src.size(), cv::INTER_CUBIC);
}

struct PreprocessorDelegate {
    /**
     * Delegate function with a copied cv::UMat pointer.
     * Needed since some OpenCV functions do not accept const references
     * and Qt cannot handle non-const reference to Mat as a metatype.
     *
     * @param frame frame to preprocess
     */
    static void preprocess_frame_delegate(Preprocessor *pp, cv::UMat frame);
};

void PreprocessorDelegate::preprocess_frame_delegate(Preprocessor *pp, cv::UMat frame) {
    // Modifier frame
    if (pp->m_modifier) { pp->m_modifier->modify(frame); }
    // Rotate frame
    rotate(frame, static_cast<double>(pp->m_rotation_angle), frame);
    // Frame zoom
    zoom(frame, frame, pp->m_zoom_factor);
    // Convert to RGB
    if (pp->m_convert_rgb) { cv::cvtColor(frame, frame, CV_BGR2RGB); }
    // Emit preprocessed frame
    Q_EMIT pp->frame_processed(frame);
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
    if (m_process_all)
    { PreprocessorDelegate::preprocess_frame_delegate(this, frame); }
    else { queue_frame(frame); }
}

void Preprocessor::queue_frame(const cv::UMat &frame) {
    // Load the frame into the single-element queue
    s_frame = frame;
    // If there is no current frame being processed, start the timer
    if (!s_queue_timer.isActive()) { s_queue_timer.start(0, this); }
}

void Preprocessor::timerEvent(QTimerEvent *ev) {
    if (ev->timerId() != s_queue_timer.timerId()) { return; }
    // Process the frame; this function is blocking and will not
    // return until the frame is processed, and usually has slower
    // execution time, which limits frame rate

    // Pass copy of pointer
    PreprocessorDelegate::preprocess_frame_delegate(this, s_frame);
    s_frame.release();
    // Stop the timer so that the next received frame can be processed
    s_queue_timer.stop();
}

double Preprocessor::get_zoom_factor() const {
    return m_zoom_factor;
}
