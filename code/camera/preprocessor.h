#ifndef MINOTAUR_CPP_PREPROCESSOR_H
#define MINOTAUR_CPP_PREPROCESSOR_H

#include <QObject>

#include <memory>

#include <opencv2/core/mat.hpp>

class VideoModifier;

class Preprocessor : public QObject {
Q_OBJECT

public:
    explicit Preprocessor(QObject *parent = nullptr);

    Q_SLOT void preprocess_frame(const cv::UMat &frame);

    Q_SLOT void zoom_changed(double zoom_factor);

    Q_SLOT void convert_rgb(bool convert_rgb);

    Q_SLOT void use_modifier(const std::shared_ptr<VideoModifier> &modifier);

    Q_SIGNAL void frame_processed(const cv::UMat &frame);

private:
    void __preprocess_frame(cv::UMat frame);

    std::shared_ptr<VideoModifier> m_modifier;

    double m_zoom_factor = 1.0;
    bool m_convert_rgb = true;
};

#endif //MINOTAUR_CPP_PREPROCESSOR_H
