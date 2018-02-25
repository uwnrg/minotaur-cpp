#ifndef MINOTAUR_CPP_CAMERADISPLAY_H
#define MINOTAUR_CPP_CAMERADISPLAY_H

#include <QDialog>

#include "camerathread.h"
#include "capture.h"
#include "converter.h"

class QVBoxLayout;

class QComboBox;

class QPushButton;

class ImageViewer;

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

    void keyPressEvent(QKeyEvent *event) override;

protected Q_SLOTS:

    void selectedCameraChanged(int list_index);

    void effectsChanged(int effect_index);

    void captureAndSave();

    void recordButtonClicked();

    void recordSaveFile();

Q_SIGNALS:

    void forwardKeyEvent(int);

    void beginRecording();

    void stopRecording();

    void recordFileAcquired(QString file, int width, int height);

private:
    void pauseVideo();

    std::unique_ptr<QVBoxLayout> m_layout;
    std::unique_ptr<QComboBox> m_camera_list;
    std::unique_ptr<QComboBox> m_effects_list;
    std::unique_ptr<QPushButton> m_capture_btn;
    std::unique_ptr<QPushButton> m_record_btn;
    std::unique_ptr<ImageViewer> m_image_viewer;

    std::unique_ptr<ActionBox> m_actions;

    int m_camera;
    int m_image_count = 0;
    int m_video_count = 0;

    Capture m_capture;
    Converter m_converter;
    IThread m_capture_thread;
    IThread m_converter_thread;
};

#endif //MINOTAUR_CPP_CAMERADISPLAY_H
