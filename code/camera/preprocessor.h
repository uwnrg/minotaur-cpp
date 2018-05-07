#ifndef MINOTAUR_CPP_PREPROCESSOR_H
#define MINOTAUR_CPP_PREPROCESSOR_H

#include <QObject>
#include <QBasicTimer>

#include <memory>

#include <opencv2/core/mat.hpp>

class VideoModifier;

/**
 * The Preprocessor is responsible for handling any modifications or
 * processes run on the raw image from the capture output before sending
 * the frame to the Converter.
 *
 * This includes VideoModifier, zoom, and rotation.
 *
 * Frame processing is as such: a frame is received from the Capture and
 * is placed in a "single-element queue". When the current frame has been
 * processed, the next frame in the queue is processed. If a frame is
 * received while the current frame is being processed, it is ignored.
 */
class Preprocessor : public QObject {
Q_OBJECT

public:
    Preprocessor();

    /**
     * Queue the frame to be preprocessed.
     *
     * @param frame the frame to preprocess
     */
    Q_SLOT void preprocess_frame(const cv::UMat &frame);

    Q_SLOT void zoom_changed(double zoom_factor);

    Q_SLOT void rotation_changed(int angle);

    Q_SLOT void convert_rgb(bool convert_rgb);

    /**
     * Replace the modifier in the class with the provided one.
     * This slot is fired when the modifier has been changed on the UI.
     *
     * @param modifier shared pointer to the new modifier
     */
    Q_SLOT void use_modifier(const std::shared_ptr<VideoModifier> &modifier);

    /**
     * Signal emitted when the frame has been processed.
     *
     * @param frame processed frame
     */
    Q_SIGNAL void frame_processed(const cv::UMat &frame);

    double get_zoom_factor() const;

private:
    /**
     * Queue a frame to be processed.
     *
     * @param frame
     */
    void queue_frame(const cv::UMat &frame);

    /**
     * Delegate function with a copied cv::UMat pointer.
     * Needed since some OpenCV functions do not accept const references
     * and Qt cannot handle non-const reference to Mat as a metatype.
     *
     * @param frame frame to preprocess
     */
    void preprocess_frame_delegate(cv::UMat frame);

    /**
     * Timer fired to trigger a frame processing.
     *
     * @param ev timer event
     */
    void timerEvent(QTimerEvent *ev) override;

    std::shared_ptr<VideoModifier> m_modifier;
    cv::UMat m_frame;

    QBasicTimer m_queue_timer;

    double m_zoom_factor;
    int m_rotation_angle;
    bool m_convert_rgb;
    bool m_process_all;
};

#endif //MINOTAUR_CPP_PREPROCESSOR_H
