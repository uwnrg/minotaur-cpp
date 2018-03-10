#ifndef MINOTAUR_CPP_IMAGEVIEWER_H_H
#define MINOTAUR_CPP_IMAGEVIEWER_H_H

#include <QObject>
#include <QImage>
#include <QWidget>

#include <opencv2/opencv.hpp>

#include "capture.h"
#include "preprocessor.h"
#include "converter.h"
#include "recorder.h"
#include "camerathread.h"

namespace Ui {
    class ImageViewer;
}

class QPaintEvent;

class CameraDisplay;

class ImageViewer : public QWidget {
    Q_OBJECT

public:
    enum {
        DEFAULT_FPS_UPDATE = 1000
    };

    explicit ImageViewer(
        CameraDisplay *parent = nullptr,
        int fps_update_interval = DEFAULT_FPS_UPDATE
    );

    ~ImageViewer() override;

    const QImage &get_image();

    Q_SLOT void set_frame_rate(double frame_rate);

    Q_SLOT void set_zoom(double zoom);

    Q_SLOT void save_screenshot(const QString &file);

    Q_SLOT void handle_recording();

    Q_SIGNAL void stop_recording();

    Q_SIGNAL void start_recording(const QString &file, int width, int height);

private:
    void timerEvent(QTimerEvent *ev) override;

    void paintEvent(QPaintEvent *ev) override;

    Ui::ImageViewer *ui;

    QImage m_image;

    Capture m_capture;
    Preprocessor m_preprocessor;
    Converter m_converter;
    Recorder m_recorder;

    IThread m_thread_capture;
    IThread m_thread_preprocessor;
    IThread m_thread_converter;
    IThread m_thread_recorder;

    QBasicTimer m_frame_timer;

    int m_fps_update_interval;
};

#endif //MINOTAUR_CPP_IMAGEVIEWER_H_H
