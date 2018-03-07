#ifndef MINOTAUR_CPP_CONVERTER_H
#define MINOTAUR_CPP_CONVERTER_H

#include <QObject>
#include <QBasicTimer>

#include <opencv2/opencv.hpp>

class ImageViewer;

class Converter : public QObject {
Q_OBJECT

public:
    explicit Converter(QObject *parent = nullptr);

    Q_SIGNAL void image_ready(const QImage &img);

    Q_SLOT void process_frame(const cv::UMat &frame);

private:
    static void matDelete(void *mat);
};


#endif //MINOTAUR_CPP_CONVERTER_H
