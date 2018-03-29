#include "fakecamera.h"
#include "globalsim.h"
#include "../gui/mainwindow.h"

FakeCamera::FakeCamera() {
    open(FAKE_CAMERA);
}

FakeCamera::~FakeCamera() = default;

cv::Rect2d FakeCamera::get_robot_rect() {
    double width = GlobalSim::Robot::WIDTH;
    vector2d loc;
    if (auto lp = Main::get()->global_sim().lock()) { loc = lp->robot(); }
    loc += {WIDTH / 2, HEIGHT / 2};
    return {loc.x() - width / 2, loc.y() - width / 2, width, width};
}

bool FakeCamera::open(const cv::String &) {
    return false;
}

bool FakeCamera::open(const cv::String &, int) {
    return false;
}

bool FakeCamera::open(int index) {
    m_open = index == FAKE_CAMERA;
    return m_open;
}

bool FakeCamera::isOpened() const {
    return m_open;
}

void FakeCamera::release() {
    m_open = false;
}

bool FakeCamera::grab() {
    return true;
}

bool FakeCamera::retrieve(cv::OutputArray image, int) {
    return false;
}

cv::VideoCapture &FakeCamera::operator>>(cv::Mat &image) {
    return *this;
}

cv::VideoCapture &FakeCamera::operator>>(cv::UMat &image) {
    cv::Rect2d robot = get_robot_rect();
    image.create(cv::Size(640, 480), CV_8UC3);
    cv::rectangle(image, {0, 0}, {640, 640}, {0, 0, 0}, cv::FILLED);
    cv::rectangle(image, robot.tl(), robot.br(), {68, 196, 98}, cv::FILLED);
    return *this;
}

bool FakeCamera::read(cv::OutputArray image) {
    return false;
}

bool FakeCamera::set(int, double) {
    return false;
}

double FakeCamera::get(int prop_id) const {
    switch (prop_id) {
        case cv::CAP_PROP_FRAME_WIDTH:
            return WIDTH;
        case cv::CAP_PROP_FRAME_HEIGHT:
            return HEIGHT;
        default:
            return 0;
    }
}
