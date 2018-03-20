#include "converter.h"
#include "imageviewer.h"

Converter::Converter(ImageViewer *image_viewer) :
    m_image_viewer(image_viewer) {}

void Converter::process_frame(const cv::UMat &frame) {
    double scale = std::min(
        static_cast<double>(m_image_viewer->width()) / frame.size().width,
        static_cast<double>(m_image_viewer->height()) / frame.size().height
    );
    cv::UMat dst;
    cv::resize(frame, dst, cv::Size(), scale, scale, cv::INTER_LINEAR);
    const QImage image(
        dst.getMat(1).data, dst.cols, dst.rows, static_cast<int>(dst.step),
        QImage::Format_RGB888, &umat_delete, new cv::UMat(dst)
    );
    ++m_frames;
    Q_EMIT image_ready(image);
}

void Converter::umat_delete(void *mat) {
    delete static_cast<cv::UMat *>(mat);
}

int Converter::get_and_reset_frames() {
    int frames = m_frames;
    m_frames = 0;
    return frames;
}
