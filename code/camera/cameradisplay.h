#ifndef MINOTAUR_CPP_CAMERADISPLAY_H
#define MINOTAUR_CPP_CAMERADISPLAY_H

#include <QDialog>
#include <QBasicTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QSpinBox>

#include "camerathread.h"
#include "capture.h"
#include "converter.h"
#include "code/gui/griddisplay.h"
#include "../video/modify.h"
#include "../video/recorder.h"

Q_DECLARE_METATYPE(std::vector<ActionButton *>);

class QVBoxLayout;

class QComboBox;

class QPushButton;

class ImageViewer;

class QSlider;

class QLabel;

class GridDisplay;

class CameraDisplay : public QDialog {
Q_OBJECT

public:
    explicit CameraDisplay(QWidget *parent = nullptr, int camera_index = 0);

    ~CameraDisplay() override;

    void setCamera(int camera);

    void update_framerate(int frames);

    int getCamera();

    int getWeighting();

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

    void update_zoom();

    void gridSelectChanged(int weight_index);

    void weightingChanged(int weighting);

    /**
     * Since the ActionButton widgets must be created and maintained in the
     * same thread as the parent ActionBox, and the Capture is running in
     * its own thread, we have to receive requests from that thread to create
     * the objects.
     *
     * This slot is called when an effect changes.
     *
     * @see CameraDisplay::returnActionButtons
     * @param num_buttons the number of action buttons needed
     */
    void requestActionButtons(int num_buttons);

Q_SIGNALS:

    void forwardKeyEvent(int);

    void beginRecording();

    void stopRecording();

    void recordFileAcquired(QString file, int width, int height);

    /**
     * Return the requested action buttons as a list of pointers.
     *
     * @param action_btns the request action buttons.
     */
    void returnActionButtons(const std::vector<ActionButton *> &action_btns, ActionBox *box);

private:
    void pauseVideo();

    void timerEvent(QTimerEvent *ev) override;

    std::unique_ptr<QVBoxLayout> m_layout;
    std::unique_ptr<QComboBox> m_camera_list;
    std::unique_ptr<QComboBox> m_effects_list;
    std::unique_ptr<QPushButton> m_capture_btn;
    std::unique_ptr<QPushButton> m_record_btn;
    std::unique_ptr<ImageViewer> m_image_viewer;
    std::unique_ptr<QSlider> m_zoom_slider;

    std::unique_ptr<GridDisplay> m_grid_display;
    std::unique_ptr<QPushButton> m_display_grid_btn;
    std::unique_ptr<QPushButton> m_hide_grid_btn;
    std::unique_ptr<QPushButton> m_deselect_btn;

    std::unique_ptr<QLabel> m_framerate_label;
    std::unique_ptr<QLabel> m_zoom_label;

    std::unique_ptr<ActionBox> m_action_box;

    std::vector<std::unique_ptr<ActionButton>> m_action_btns;
    std::vector<ActionButton *> m_action_btn_ptrs;

    std::unique_ptr<QComboBox> m_weight_list;
    std::unique_ptr<QSpinBox> m_weight_selector;

    int m_camera;
    int m_image_count = 0;
    int m_video_count = 0;

    Capture m_capture;
    Converter m_converter;
    IThread m_capture_thread;
    IThread m_converter_thread;

    QBasicTimer m_framerate_timer;

    int maxWeight = 10;
    QString weightSelected;
    int weighting = 0;
};

#endif //MINOTAUR_CPP_CAMERADISPLAY_H
