#ifndef MINOTAUR_CPP_RECORDER_H
#define MINOTAUR_CPP_RECORDER_H

#include <QObject>

#include <memory>
#include <opencv2/videoio.hpp>

class Recorder : public QObject {
Q_OBJECT

public:
    explicit Recorder(
        double frame_rate = 30,
        bool color = true
    );

    Q_SLOT void start_recording(const QString &file, int width, int height);

    Q_SLOT void stop_recording();

    Q_SLOT void frame_received(const cv::UMat &img);

    bool is_recording() const;

private:
    std::unique_ptr<cv::VideoWriter> m_video_writer;

    double m_frame_rate;
    bool m_color;
    bool m_recording;
};

#endif //MINOTAUR_CPP_RECORDER_H
