#ifndef MINOTAUR_CPP_CAMERADISPLAY_H
#define MINOTAUR_CPP_CAMERADISPLAY_H

#include <QObject>
#include <QDialog>
#include <QVBoxLayout>
#include <QComboBox>

#include "imageviewer.h"
#include "camerathread.h"
#include "capture.h"
#include "converter.h"

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

    Q_SIGNALS:
    void forwardKeyEvent(int);

private:
    void pauseVideo();

    QVBoxLayout *m_layout;
    QComboBox *m_camera_list;
    QComboBox *m_effects_list;
    QPushButton *m_capture_btn;
    ImageViewer *m_image_viewer;

    int m_camera;
    int m_image_count = 0;

    Capture m_capture;
    Converter m_converter;
    IThread m_capture_thread;
    IThread m_converter_thread;
};

#endif //MINOTAUR_CPP_CAMERADISPLAY_H
