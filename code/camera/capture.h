#ifndef MINOTAUR_CPP_CAPTURE_H
#define MINOTAUR_CPP_CAPTURE_H

#include <QObject>
#include <QBasicTimer>
#include <memory>

// OpenCV forward declarations
namespace cv {
    class UMat;
    class VideoCapture;
}

/**
 * This object is the beginning of the image pipeline and
 * is responsible for managing the OpenCV Video Capture
 * object that polls frames from the active camera. These frames
 * are emitted to the preprocessor.
 */
class Capture : public QObject {
    Q_OBJECT

public:
    Capture();

    int capture_width() const;

    int capture_height() const;

    Q_SIGNAL void capture_started();

    Q_SIGNAL void capture_stopped();

    /**
     * Signal emitted when a frame has been received
     * by the Capture.
     */
    Q_SIGNAL void frame_ready(const cv::UMat &);

    Q_SLOT void start_capture(int cam);

    Q_SLOT void stop_capture();

    Q_SLOT void change_camera(int camera);

private:
    void timerEvent(QTimerEvent *ev) override;

    /**
     * Timer fires at a fixed interval to pull images
     * from the capture.
     */
    QBasicTimer m_timer;
    /**
     * Video Capture instance that produces images.
     */
    std::unique_ptr<cv::VideoCapture> m_video_capture;
};

#endif //MINOTAUR_CPP_CAPTURE_H
