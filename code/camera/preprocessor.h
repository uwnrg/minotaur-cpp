#ifndef MINOTAUR_CPP_PREPROCESSOR_H
#define MINOTAUR_CPP_PREPROCESSOR_H

#include <QObject>
#include <QBasicTimer>

#include <memory>

#include <opencv2/core/mat.hpp>

class VideoModifier;

class Preprocessor : public QObject {
Q_OBJECT

public:
    Preprocessor();

    Q_SLOT void preprocess_frame(const cv::UMat &frame);

    Q_SLOT void zoom_changed(double zoom_factor);

    Q_SLOT void rotation_changed(int angle);

    Q_SLOT void toggle_rotation();

    Q_SLOT void convert_rgb(bool convert_rgb);

    Q_SLOT void use_modifier(const std::shared_ptr<VideoModifier> &modifier);

    Q_SIGNAL void update_rotation_ui(int value);

    Q_SIGNAL void frame_processed(const cv::UMat &frame);

private:
    void __queue(const cv::UMat &frame);

    void __preprocess_frame(cv::UMat frame);

    void timerEvent(QTimerEvent *ev) override;

    std::shared_ptr<VideoModifier> m_modifier;
    cv::UMat m_frame;

    QBasicTimer m_queue_timer;

    double m_zoom_factor = 1.0;
    int m_rotation_angle = 0;
    bool m_rotate = false;
    bool m_convert_rgb = true;
    bool m_process_all = false;
};

#endif //MINOTAUR_CPP_PREPROCESSOR_H
