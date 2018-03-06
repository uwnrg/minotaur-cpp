#ifndef MINOTAUR_CPP_RECORDER_H
#define MINOTAUR_CPP_RECORDER_H

#include <opencv2/opencv.hpp>
#include <QObject>

#include "modify.h"

class Recorder : public QObject {
Q_OBJECT

public:
    Recorder(const std::string &file_name, int four_cc, double frame_rate, cv::Size size, bool is_color = true);

    Q_SLOT void image_received(cv::UMat &img);

    Q_SLOT void stop_recording();

    bool is_recording();

private:
    cv::VideoWriter m_video_writer;

    bool m_is_recording;
};

#endif //MINOTAUR_CPP_RECORDER_H
