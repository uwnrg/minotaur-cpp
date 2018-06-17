#include <opencv2/imgproc.hpp>
#include "converter.h"
#include "imageviewer.h"

static void umat_delete(void *mat)
{ delete static_cast<cv::UMat *>(mat); }

Converter::Converter(ImageViewer *image_viewer) :
    m_scale(1.0),
    m_image_viewer(image_viewer) {}

void Converter::process_frame(const cv::UMat &frame) {
    // Calculate the required scale
    m_scale = std::min(
        static_cast<double>(m_image_viewer->width()) / frame.size().width,
        static_cast<double>(m_image_viewer->height()) / frame.size().height
    );
    cv::UMat dst;
    // Scale the image into dst
    cv::resize(frame, dst, cv::Size(), m_scale, m_scale, cv::INTER_LINEAR);
    // Convert to QImage
    const QImage image(
        dst.getMat(1).data, dst.cols, dst.rows, static_cast<int>(dst.step),
        QImage::Format_RGB888, &umat_delete, new cv::UMat(dst)
    );
    // Increment number of frames processed
    ++m_frames;
    // Emit the image
    Q_EMIT image_ready(image);
}

int Converter::get_and_reset_frames() {
    int frames = m_frames;
    m_frames = 0;
    return frames;
}

double Converter::get_previous_scale() const {
    return m_scale;
}
