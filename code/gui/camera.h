#ifndef MINOTAUR_CPP_CAMERA_H
#define MINOTAUR_CPP_CAMERA_H

#include <memory>

#include <opencv2/opencv.hpp>

#include <QWidget>
#include <QDialog>
#include <QThread>
#include <QBasicTimer>
#include <QArgument>
#include <QMetaType>
#include <QScopedPointer>
#include <QTimerEvent>
#include <QImage>
#include <QPaintEvent>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>

Q_DECLARE_METATYPE(cv::Mat);

class CameraDisplay;

class Capture : public QObject {
Q_OBJECT

public:
    explicit Capture(QObject *parent = nullptr);

    Q_SIGNAL void started();

    Q_SIGNAL void matReady(const cv::Mat &);

    Q_SLOT void start(int cam);

    Q_SLOT void stop();

private:
    void timerEvent(QTimerEvent *ev) override;

    QBasicTimer m_timer;
    QScopedPointer<cv::VideoCapture> m_video_capture;
};

class Converter : public QObject {
Q_OBJECT

public:
    explicit Converter(QObject *parent = nullptr, CameraDisplay *display = nullptr);

    void setProcessAll(bool process_all);

    Q_SIGNAL void imageReady(const QImage &);

    Q_SLOT void processFrame(const cv::Mat &frame);

private:
    static void matDelete(void *mat);

    void queue(const cv::Mat &frame);

    void process(cv::Mat frame);

    void timerEvent(QTimerEvent *ev) override;

    CameraDisplay *m_display;
    QBasicTimer m_timer;
    cv::Mat m_frame;
    bool m_process_all = true;
};

class ImageViewer : public QWidget {
Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = nullptr);

    const QImage &getImage();

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
    explicit CameraDisplay(QWidget *parent = nullptr, int camera_index = 0);

    ~CameraDisplay() override;

    void setCamera(int camera);

    int getCamera();

protected:
    void setVisible(bool visible) override;

    void reject() override;

protected Q_SLOTS:
    void selectedCameraChanged(int list_index);

	void captureAndSave();

private:
    void pauseVideo();

    QVBoxLayout *m_layout;
    QComboBox *m_camera_list;
	QPushButton *m_capture_btn;
    ImageViewer *m_image_viewer;

    int m_camera;
    int m_image_count = 0;

    Capture m_capture;
    Converter m_converter;
    IThread m_capture_thread;
    IThread m_converter_thread;
};

#endif //MINOTAUR_CPP_CAMERA_H
