#ifndef MINOTAUR_CPP_CONVERTER_H
#define MINOTAUR_CPP_CONVERTER_H

#include <QObject>
#include <QBasicTimer>

#include <opencv2/opencv.hpp>

#include "../video/modify.h"

class CameraDisplay;

class Converter : public QObject {
    Q_OBJECT

public:
    explicit Converter(QObject *parent = nullptr, CameraDisplay *display = nullptr);

    void setProcessAll(bool process_all);

    Q_SIGNAL void imageReady(const QImage &);

    Q_SLOT void processFrame(const cv::Mat &frame);

    Q_SLOT void modifierChanged(int modifier_index);

    Q_SLOT void imageKeyEvent(int key);

private:
    static void matDelete(void *mat);

    void queue(const cv::Mat &frame);

    void process(cv::Mat frame);

    void timerEvent(QTimerEvent *ev) override;

    CameraDisplay *m_display;
    QBasicTimer m_timer;

    cv::Mat m_frame;
    std::unique_ptr<VideoModifier> m_modifier;

    bool m_process_all = true;
};


#endif //MINOTAUR_CPP_CONVERTER_H
