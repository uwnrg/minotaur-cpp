#ifndef MINOTAUR_CPP_FAKECAMERA_H
#define MINOTAUR_CPP_FAKECAMERA_H

#include <QObject>
#include <opencv2/videoio.hpp>

/**
 * Mocked VideoCapture class for use with simulated robot and
 * frame production.
 */
class FakeCamera : public QObject, public cv::VideoCapture {
Q_OBJECT

public:
    enum {
        FAKE_CAMERA = -1,
        WIDTH = 640,
        HEIGHT = 480
    };

    FakeCamera();
    ~FakeCamera() override;

    static cv::Rect2d get_robot_rect();
    static cv::Point2d get_object_rect();

    bool open(const cv::String &filename) override;
    bool open(const cv::String &filename, int api_pref) override;
    bool open(int index) override;

    bool isOpened() const override;
    void release() override;

    bool grab() override;
    bool retrieve(cv::OutputArray image, int flag) override;

    cv::VideoCapture& operator>>(cv::Mat &image) override;
    cv::VideoCapture& operator>>(cv::UMat &image) override;

    bool read(cv::OutputArray image) override;
    bool set(int prop_id, double value) override;
    double get(int prop_id) const override;

private:
    bool m_open;
};

#endif //MINOTAUR_CPP_FAKECAMERA_H
