#ifndef MINOTAUR_CPP_CAMERADISPLAY_H
#define MINOTAUR_CPP_CAMERADISPLAY_H

#include <memory>
#include <QDialog>

#include "actionbox.h"
#include "imageviewer.h"

namespace Ui {
    class CameraDisplay;
}

class CameraDisplay : public QDialog {
Q_OBJECT

public:
    explicit CameraDisplay(QWidget *parent = nullptr);

    ~CameraDisplay() override;

    void setVisible(bool visible) override;

    void reject() override;

    Q_SLOT void camera_box_changed(int camera);

    Q_SLOT void effect_box_changed(int effect);

    Q_SLOT void take_screen_shot();

    Q_SLOT void record_clicked();

    Q_SLOT void update_zoom(int value);

    Q_SIGNAL void display_opened(int camera);

    Q_SIGNAL void display_closed();

    Q_SIGNAL void camera_changed(int camera);

    Q_SIGNAL void effect_changed(const std::shared_ptr<VideoModifier> &effect);

    Q_SIGNAL void save_screenshot(const QString &file);

    Q_SIGNAL void zoom_changed(double zoom);

    Q_SIGNAL void toggle_recording();

private:
    Ui::CameraDisplay *ui;

    std::unique_ptr<ActionBox> m_action_box;
    std::unique_ptr<ImageViewer> m_image_viewer;
};

#endif //MINOTAUR_CPP_CAMERADISPLAY_H
