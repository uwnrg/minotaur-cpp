#ifndef MINOTAUR_CPP_CAMERA_H
#define MINOTAUR_CPP_CAMERA_H

#include <opencv2/opencv.hpp>

#include <QWidget>
#include <QDialog>
#include <QThread>
#include <QtCore/QBasicTimer>
#include <QtCore/QArgument>
#include <QtCore/QMetaType>
#include <QtCore/QScopedPointer>
#include <QtCore/QTimerEvent>
#include <QtGui/QImage>
#include <QtGui/QPaintEvent>
#include <QtWidgets/QVBoxLayout>

Q_DECLARE_METATYPE(cv::Mat);

class Capture : public QObject {
Q_OBJECT

public:
    explicit Capture(QObject *parent = nullptr);

    Q_SIGNAL void started();

    Q_SIGNAL void matReady(const cv::Mat &);

    Q_SLOT void start(int cam = 0);

    Q_SLOT void stop();

private:
    void timerEvent(QTimerEvent *ev) override;

    QBasicTimer m_timer;
    QScopedPointer<cv::VideoCapture> m_video_capture;
};

class Converter : public QObject {
Q_OBJECT

public:
    explicit Converter(QObject *parent = nullptr);

    void setProcessAll(bool process_all);

    Q_SIGNAL void imageReady(const QImage &);

    Q_SLOT void processFrame(const cv::Mat &frame);

private:
    static void matDelete(void *mat);

    void queue(const cv::Mat &frame);

    void process(cv::Mat frame);

    void timerEvent(QTimerEvent *ev) override;

    QBasicTimer m_timer;
    cv::Mat m_frame;
    bool m_process_all = true;
};

class ImageViewer : public QWidget {
Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = nullptr);

    Q_SLOT void setImage(const QImage &img);

private:
    void paintEvent(QPaintEvent *) override;

    QImage m_img;
};

class IThread final : public QThread {
public:
    ~IThread() override;
};

class CameraDisplay : public QDialog {
Q_OBJECT

public:
    explicit CameraDisplay(QWidget *parent = nullptr, int camera = 0);

    ~CameraDisplay() override;

    void setCamera(int camera);
    int getCamera();


protected:
    void setVisible(bool visible) override;

    void reject() override;

private:
    QVBoxLayout *m_layout;
    ImageViewer *m_image_viewer;

    int m_camera;

    Capture m_capture;
    Converter m_converter;
    IThread m_capture_thread;
    IThread m_converter_thread;
};

#endif //MINOTAUR_CPP_CAMERA_H
