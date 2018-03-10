#include "converter.h"
#include "imageviewer.h"

Converter::Converter(ImageViewer *image_viewer) :
    m_image_viewer(image_viewer) {}

void Converter::process_frame(const cv::UMat &frame) {
    auto *ptr = dynamic_cast<QWidget *>(parent());
#ifndef NDEBUG
    if (ptr) {
#endif
    double scale = std::min(
        static_cast<double>(ptr->width()) / frame.size().width,
        static_cast<double>(ptr->height()) / frame.size().height
    );
    cv::resize(frame, frame, cv::Size(), scale, scale, cv::INTER_LINEAR);
#ifndef NDEBUG
    }
#endif
    const QImage image(
        frame.getMat(1).data, frame.cols, frame.rows, static_cast<int>(frame.step),
        QImage::Format_RGB888, &umat_delete, new cv::UMat(frame)
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
