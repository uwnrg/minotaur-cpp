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

    Q_SLOT void convert_rgb(bool convert_rgb);

    Q_SLOT void use_modifier(const std::shared_ptr<VideoModifier> &modifier);

    Q_SIGNAL void frame_processed(const cv::UMat &frame);

    double get_zoom_factor() const;

private:
    void queue_frame(const cv::UMat &frame);

    void preprocess_frame_delegate(cv::UMat frame);

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
