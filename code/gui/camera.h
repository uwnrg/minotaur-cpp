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
#include <QGraphicsScene>
#include <QGraphicsView>

#include "../video/modify.h"
#include "../video/recorder.h"
#include <QLabel>

#include "../video/modify.h"
#include "../video/recorder.h"

Q_DECLARE_METATYPE(cv::Mat);

class CameraDisplay;

class GridDisplay;

class Capture : public QObject {
Q_OBJECT

public:
    explicit Capture(QObject *parent = nullptr);

    int capture_width();

    int capture_height();

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

    /**
     * Signal emitted after a cv::Mat frame has been
     * read from the VideoCapture and modified.
     *
     * @param img reference to processed image
     */
    Q_SIGNAL void frameProcessed(cv::Mat &img);

    Q_SLOT void startRecording(QString file, int width, int height);

    Q_SLOT void stopRecording();

    Q_SLOT void processFrame(const cv::Mat &frame);

    Q_SLOT void modifierChanged(int modifier_index);

    Q_SLOT void imageKeyEvent(int key);

    bool is_recording();

    int getFrames();

private:
    static void matDelete(void *mat);

    void queue(const cv::Mat &frame);

    void process(cv::Mat frame);

    void timerEvent(QTimerEvent *ev) override;

    CameraDisplay *m_display;
    QBasicTimer m_timer;

    int m_frames = 0;

    cv::Mat m_frame;
    std::unique_ptr<VideoModifier> m_modifier;
    std::unique_ptr<Recorder> m_recorder;

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

    void updateFramerate(int frames);

    int getCamera();


protected:
    void setVisible(bool visible) override;

    void reject() override;

    void keyPressEvent(QKeyEvent *event) override;

Q_SIGNALS:
    void beginRecording();

    void stopRecording();

    void recordFileAcquired(QString file, int width, int height);

protected Q_SLOTS:

    void selectedCameraChanged(int list_index);

    void effectsChanged(int effect_index);

    void captureAndSave();

    void recordButtonClicked();

    void recordSaveFile();

Q_SIGNALS:
    void forwardKeyEvent(int);

private:
    void pauseVideo();

    void timerEvent(QTimerEvent *ev) override;

    QVBoxLayout *m_layout;
    QComboBox *m_camera_list;
    QComboBox *m_effects_list;
    QPushButton *m_capture_btn;
    QPushButton *m_record_btn;
    ImageViewer *m_image_viewer;

    QPushButton *m_display_grid_btn;
    QPushButton *m_hide_grid_btn;
    GridDisplay *m_grid_display;
	QPushButton *m_deselect_btn;

    QLabel *m_framerate_label;
    QBasicTimer m_framerate_timer;

    int m_camera;
    int m_image_count = 0;
    int m_video_count = 0;

    Capture m_capture;
    Converter m_converter;
    IThread m_capture_thread;
    IThread m_converter_thread;
};

#endif //MINOTAUR_CPP_CAMERA_H
