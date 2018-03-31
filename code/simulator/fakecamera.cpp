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

cv::Point2d FakeCamera::get_object_rect() {
    vector2d loc;
    if (auto lp = Main::get()->global_sim().lock()) { loc = lp->object(); }
    loc += {WIDTH / 2, HEIGHT / 2};
    return {loc.x(), loc.y()};
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

void gaussian_noise(cv::UMat &image) {
    cv::UMat noise(image.size(), CV_16SC3);
    cv::randn(noise, cv::Scalar::all(0), cv::Scalar::all(10));
    cv::UMat temp;
    image.convertTo(temp, CV_16SC3);
    cv::addWeighted(temp, 1, noise, 1, 0, temp);
    temp.convertTo(image, image.type());
}

cv::VideoCapture &FakeCamera::operator>>(cv::UMat &image) {
    cv::Rect2d robot = get_robot_rect();
    cv::Rect2d robot_l0 = robot;
    robot_l0.x += 2;
    robot_l0.y += 2;
    robot_l0.width -= 4;
    robot_l0.height -= 4;
    cv::Point2d object = get_object_rect();
    int width = GlobalSim::Robot::WIDTH;
    image.create(cv::Size(640, 480), CV_8UC3);
    // Draw background
    cv::rectangle(image, {0, 0}, {640, 640}, {0, 0, 0}, cv::FILLED);
    // Draw robot
    cv::rectangle(image, robot.tl(), robot.br(), {68, 196, 98}, cv::FILLED);
    cv::rectangle(image, robot_l0.tl(), robot_l0.br(), {66, 244, 167}, cv::FILLED);
    // Draw object
    cv::ellipse(image, object, {width * 3 / 4, width / 2}, 0, 0, 360, {66, 134, 244}, cv::FILLED);
    cv::ellipse(image, object, {(width - 4) * 3 / 4, (width - 4) / 2}, 0, 0, 360, {66, 217, 244}, cv::FILLED);
    // Add noise to image
    gaussian_noise(image);
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
