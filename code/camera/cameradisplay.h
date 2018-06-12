#ifndef MINOTAUR_CPP_CAMERADISPLAY_H
#define MINOTAUR_CPP_CAMERADISPLAY_H

#include <memory>
#include <QDialog>
#include <QSlider>

#include "actionbox.h"
#include "imageviewer.h"

namespace Ui {
    class CameraDisplay;
}

/**
 * This Dialog is responsible for handling the camera input, robot and object movement
 * input, and some video modification input. Most of competition-related activity
 * happens in this window.
 *
 * CameraDisplay houses ImageViewer, which displays the camera stream.
 */
class CameraDisplay : public QDialog {
Q_OBJECT

public:
    explicit CameraDisplay(QWidget *parent = nullptr);

    ~CameraDisplay() override;

    void setVisible(bool visible) override;

    void reject() override;

    /**
     * @return the currently selected grid weighting
     */
    int get_weighting();

    void setup_slider(
        QSlider &slider, int value, 
        int min, int max, int interval,
        bool tracking);

    /**
     * Slot called when the weight list has changed.
     *
     * @param weight the new weight value
     */
    Q_SLOT void weighting_changed(int weight);

    /**
     * Slot called when the selected camera has changed.
     *
     * @param camera the new camera index
     */
    Q_SLOT void camera_box_changed(int camera);

    /**
     * Slot called when the selected video modifier is changed.
     *
     * @param effect the new modifier index
     */
    Q_SLOT void effect_box_changed(int effect);

    /**
     * This slot is called when the user clicks the screenshot
     * button. This method creates the FileDialog to get the
     * image save path.
     */
    Q_SLOT void take_screen_shot();

    /**
     * Slot called when the zoom slider changes value. The value
     * from the slider must be scaled to the correct zoom level.
     *
     * @param value the raw slider zoom value
     */
    Q_SLOT void update_zoom(int value);

    /**
     * Slot called when the rotation slider has changed. The value
     * provided is raw.
     *
     * @param value raw rotation value
     */
    Q_SLOT void rotation_slider_changed(int value);

    /**
     * Slot called when the rotation degree text edit has changed.
     */
    Q_SLOT void rotation_box_changed();

    /**
     * Slot called to set the rotation value displayed in the
     * rotation text edit in case it is changed elsewhere; i.e.,
     * by the slider or by the rotation play.
     *
     * @param value rotation value to set
     */
    Q_SLOT void set_rotation(int value);

    /**
     * Slot fired by a timer to increment the value of the rotation
     * when playing rotation.
     */
    Q_SLOT void increment_rotation();

    /**
     * Slot fired when the play button has been clicked.
     *
     * @param checked whether the button is depressed
     */
    Q_SLOT void pressed_play(bool checked);

    /**
     * Slot fired when the grid select mode has changed.
     *
     * @param weight_index index of the new mode
     */
    Q_SLOT void grid_select_changed(int weight_index);

    Q_SLOT void grid_slider_moved(int value);

    Q_SLOT void show_grid_button_pushed();

    Q_SLOT void hide_grid_button_pushed();

    /**
     * Signal fired when the CameraDisplay opens so that
     * the ImageViewer can be started.
     *
     * @param camera the index of the desired camera
     */
    Q_SIGNAL void display_opened(int camera);

    /**
     * Signal fired when the CameraDisplay closes so that
     * the ImageViewer can stop the camera feed.
     */
    Q_SIGNAL void display_closed();

    /**
     * Signal fired with the newly selected camera index.
     *
     * @param camera camera index
     */
    Q_SIGNAL void camera_changed(int camera);

    /**
     * Signal fired with a shared pointer to the newly
     * selected video modifier. The preprocessor grabs and
     * holds onto this pointer in the video pipeline.
     *
     * @param effect
     */
    Q_SIGNAL void effect_changed(const std::shared_ptr<VideoModifier> &effect);

    /**
     * Signal fired to save a screenshot with the particular path.
     *
     * @param file screenshot save path
     */
    Q_SIGNAL void save_screenshot(const QString &file);

    /**
     * Signal fired with the scaled zoom value. The preprocessor grabs
     * this value and saves it to apply a zoom in the image pipeline.
     *
     * @param zoom scaled zoom value
     */
    Q_SIGNAL void zoom_changed(double zoom);

    /**
     * Signal fired when the rotation value is changed, with a normalized
     * degree value. The preprocessor grabs this value to apply a rotation.
     *
     * @param angle the rotation angle
     */
    Q_SIGNAL void rotation_changed(int angle);

    /**
     * Signal fired to toggle recording of the video feed. This signal
     * is received by the recorder object.
     */
    Q_SIGNAL void toggle_record();

    /**
     * Signal fired to turn on or off rotation play.
     *
     * @param rotate whether to play rotation
     */
    Q_SIGNAL void toggle_rotation(bool rotate);

    /**
     * Signal fired to tell the grid display to show the
     * buttons and grid elements, and accept input.
     */
    Q_SIGNAL void show_grid();

    /**
     * Signal fired to tell the grid display to hide buttons
     * and grid elements.
     */
    Q_SIGNAL void hide_grid();

    /**
     * Signal fired to tell the grid display to clear selected
     * grid elements.
     */
    Q_SIGNAL void clear_grid();

    /**
     * Signal fired to clear the selected robot or object path.
     */
    Q_SIGNAL void clear_path();

    /**
     * Signal fired to set path from grid display.
     */
    Q_SIGNAL void set_grid_path();

    /**
     * Signal fired to toggle selecting the robot or object
     * path in the ImageViewer.
     *
     * @param checked whether to select path
     */
    Q_SIGNAL void toggle_path(bool checked);

    /**
     * Signal fired when the grid selection type changes.
     *
     * @param weight_selected grid selection type
     */
    Q_SIGNAL void select_position(QString weight_selected);

    Q_SIGNAL void move_grid(double x, double y);

private:
    enum Zoom {
        ZOOM_INTERVAL = 2,
        ZOOM_MAX = 40,
        ZOOM_MIN = 10
    };

    enum Rotation {
        ROTATION_INTERVAL = 45,
        ROTATION_MIN = -180,
        ROTATION_MAX = 180
    };

    enum GridLocation {
        GRIDLOCATION_INTERVAL = 10,
        GRIDLOCATION_MIN = -100,
        GRIDLOCATION_MAX = 100
    };

    enum Grid {
        MAX_WEIGHT = 10,
        MIN_WEIGHT = -1
    };

    Ui::CameraDisplay *m_ui;

    /**
     * CameraDisplay manages the ActionBox instance to which
     * VideoModifiers attach buttons.
     */
    std::unique_ptr<ActionBox> m_action_box;
    /**
     * ImageViewer instance.
     */
    std::unique_ptr<ImageViewer> m_image_viewer;

    /**
     * The currently selected grid weighting.
     */
    int m_weighting;

};

#endif //MINOTAUR_CPP_CAMERADISPLAY_H
