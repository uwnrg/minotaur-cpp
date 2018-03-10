#ifndef MINOTAUR_CPP_CONVERTER_H
#define MINOTAUR_CPP_CONVERTER_H

#include <QObject>
#include <QBasicTimer>

#include <opencv2/opencv.hpp>

class ImageViewer;

class Converter : public QObject {
Q_OBJECT

public:
    explicit Converter(ImageViewer *image_viewer);

    Q_SIGNAL void image_ready(const QImage &img);

    Q_SLOT void process_frame(const cv::UMat &frame);

    int get_and_reset_frames();

private:
    static void umat_delete(void *mat);

    int m_frames;
    ImageViewer *m_image_viewer;
};


#endif //MINOTAUR_CPP_CONVERTER_H
