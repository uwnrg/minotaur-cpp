#ifndef MINOTAUR_CPP_CAPTURE_H
#define MINOTAUR_CPP_CAPTURE_H

#include <QObject>
#include <QBasicTimer>

#include <opencv2/opencv.hpp>

Q_DECLARE_METATYPE(cv::UMat);

class Capture : public QObject {
    Q_OBJECT

public:
    Capture();

    int capture_width() const;

    int capture_height() const;

    Q_SIGNAL void capture_started();

    Q_SIGNAL void capture_stopped();

    Q_SIGNAL void frame_ready(const cv::UMat &);

    Q_SLOT void start_capture(int cam);

    Q_SLOT void stop_capture();

private:
    void timerEvent(QTimerEvent *ev) override;

    QBasicTimer m_timer;
    std::unique_ptr<cv::VideoCapture> m_video_capture;
};

#endif //MINOTAUR_CPP_CAPTURE_H
