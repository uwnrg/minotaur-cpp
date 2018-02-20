#ifndef MINOTAUR_CPP_CAPTURE_H
#define MINOTAUR_CPP_CAPTURE_H

#include <QObject>
#include <QBasicTimer>

#include <opencv2/opencv.hpp>

Q_DECLARE_METATYPE(cv::Mat);
Q_DECLARE_METATYPE(cv::UMat);

class Capture : public QObject {
    Q_OBJECT

public:
    explicit Capture(QObject *parent = nullptr);

    Q_SIGNAL void started();

    Q_SIGNAL void matReady(const cv::UMat &);

    Q_SLOT void start(int cam);

    Q_SLOT void stop();

private:
    void timerEvent(QTimerEvent *ev) override;

    QBasicTimer m_timer;
    QScopedPointer<cv::VideoCapture> m_video_capture;
};

#endif //MINOTAUR_CPP_CAPTURE_H
