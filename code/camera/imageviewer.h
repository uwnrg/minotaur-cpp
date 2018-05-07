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

#include "../gui/griddisplay.h"
#include "../utility/vector.h"

namespace Ui {
    class ImageViewer;
}

class QPaintEvent;
class CameraDisplay;
class GridDisplay;
class GridDisplay;

/**
 * The ImageViewer is a widget responsible for displaying the images captured
 * in the image pipeline. The class also manages the elements of the pipeline,
 * and handles most connections.
 */
class ImageViewer : public QWidget {
    Q_OBJECT

public:
    enum {
        // Time in milliseconds between each framerate update
        FRAMERATE_UPDATE_INTERVAL = 1000,
        // Time in milliseconds between each rotation update
        ROTATE_UPDATE_INTERVAL = 25
    };

    explicit ImageViewer(CameraDisplay *parent = nullptr);

    ~ImageViewer() override;

    /**
     * Set the image that is displayed by the image viewer. This slot is
     * called with a newly converted QImage from Converter
     *
     * @param img frame image to display
     */
    Q_SLOT void set_image(const QImage &img);

    /**
     * Set the frame rate value that is displayed in the frame rate
     * indicator.
     *
     * @param frame_rate frame rate value
     */
    Q_SLOT void set_frame_rate(double frame_rate);

    /**
     * Set the zoom value displayed in the zoom indicator and forward
     * the value to the preprocessor.
     *
     * @param zoom scaled zoom value
     */
    Q_SLOT void set_zoom(double zoom);

    /**
     * Slot fired as a request to save the current QImage
     * to the provided file.
     *
     * @param file path to save the image
     */
    Q_SLOT void save_screenshot(const QString &file);

    /**
     * Slot called to toggle recording.
     */
    Q_SLOT void handle_recording();

    /**
     * Slot called to clear the currently selected robot or object path.
     */
    Q_SLOT void clear_path();

    /**
     * Slot called to enable or disable path selection.
     *
     * @param toggle_path whether paths should be selcted
     */
    Q_SLOT void toggle_path(bool toggle_path);

    /**
     * Slot called to enable or disable play rotation/
     *
     * @param rotate whether rotation should occur
     */
    Q_SLOT void toggle_rotation(bool rotate);

    /**
     * Signal fired to indicate that rotation values should be incremented.
     */
    Q_SIGNAL void increment_rotation();

    /**
     * Signal fired to indicate to the Recorder to stop recording.
     */
    Q_SIGNAL void stop_recording();

    /**
     * Signal fired to indicate to the Recorder to start recording.
     *
     * @param file   the video save path
     * @param width  the video pixel width
     * @param height the video pixel height
     */
    Q_SIGNAL void start_recording(const QString &file, int width, int height);

private:
    /**
     * Handle mouse click events to add to the path.
     *
     * @param ev mouse event
     */
    void mousePressEvent(QMouseEvent *ev) override;

    /**
     * Handle the framerate and rotation timers.
     *
     * @param ev timer event
     */
    void timerEvent(QTimerEvent *ev) override;

    /**
     * Paint event should draw the QImage and the paths.
     *
     * @param ev paint event
     */
    void paintEvent(QPaintEvent *ev) override;

    Ui::ImageViewer *ui;

    std::unique_ptr<GridDisplay> m_grid_display;

    /**
     * The currently displayed image.
     */
    QImage m_image;

    // Pipeline elements
    Capture m_capture;
    Preprocessor m_preprocessor;
    Converter m_converter;
    Recorder m_recorder;

    // Each of the pipelines elements belong in a thread
    IThread m_thread_capture;
    IThread m_thread_preprocessor;
    IThread m_thread_converter;
    IThread m_thread_recorder;

    /**
     * Timer fired to increment rotation.
     */
    QBasicTimer m_rotation_timer;
    /**
     * Timer fired to grab the number of processed frames
     * and display them.
     */
    QBasicTimer m_frame_timer;

    /**
     * Whether mouse events should be handled to add path nodes.
     */
    bool m_selecting_path;
};

#endif //MINOTAUR_CPP_IMAGEVIEWER_H_H
