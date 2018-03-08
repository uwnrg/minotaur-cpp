#ifndef MINOTAUR_CPP_CONVERTER_H
#define MINOTAUR_CPP_CONVERTER_H

#include <QObject>
#include <QBasicTimer>

#include <opencv2/opencv.hpp>

class CameraDisplay;

class Converter : public QObject {
    Q_OBJECT

public:
    explicit Converter(QWidget *parent = nullptr);

    Q_SIGNAL void image_ready(const QImage &img);

    Q_SLOT void process_frame(const cv::UMat &frame);

    int get_and_reset_frames();

private:
    static void umat_delete(void *mat);

    int m_frames;
};


#endif //MINOTAUR_CPP_CONVERTER_H
