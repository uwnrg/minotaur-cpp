#include "imageviewer.h"
#include "ui_imageviewer.h"

#include "cameradisplay.h"
#include "camerathread.h"
#include "capture.h"
#include "converter.h"
#include "preprocessor.h"
#include "recorder.h"

#include "../compstate/compstate.h"
#include "../compstate/parammanager.h"
#include "../controller/astar.h"
#include "../gui/global.h"
#include "../gui/griddisplay.h"
#include "../utility/logger.h"

#include <opencv2/videoio.hpp>
#include <QPainter>
#include <QBasicTimer>
#include <QFileDialog>
#include <QMouseEvent>

// Static instances of camera threads
static IThread s_thread_capture;
static IThread s_thread_preprocessor;
static IThread s_thread_converter;
static IThread s_thread_recorder;

// Timer fired to increment rotation.
static QBasicTimer s_rotation_timer;
// Timer fired to grab the number of processed frames and display them
static QBasicTimer s_frame_timer;

enum {
    // Time in milliseconds between each framerate update
    FRAMERATE_UPDATE_INTERVAL = 1000,
    // Time in milliseconds between each rotation update
    ROTATE_UPDATE_INTERVAL = 25
};

static QString color_format(double value, const QString &suffix = "") {
    return QString("<font color=\"#8ae234\">%1%2</font>").arg(value).arg(suffix);
}

ImageViewer::ImageViewer(CameraDisplay *parent) :
    QWidget(parent),

    ui(new Ui::ImageViewer),

    m_grid_display(std::make_unique<GridDisplay>(this, parent)),

    m_capture(std::make_unique<Capture>()),
    m_preprocessor(std::make_unique<Preprocessor>()),
    m_converter(std::make_unique<Converter>(this)),
    m_recorder(std::make_unique<Recorder>()),

    m_selecting_path(false) {

    ui->setupUi(this);
    // Lower the labels so that they do not block mouse events to the
    // GridDisplay and path selection
    ui->zoom_label->lower();
    ui->fps_label->lower();

    // Opaque paint event used to draw the image
    setAttribute(Qt::WA_OpaquePaintEvent);

    // Start threads and move image pipeline objects to threads
    s_thread_capture.start();
    s_thread_preprocessor.start();
    s_thread_converter.start();
    s_thread_recorder.start();
    m_capture->moveToThread(&s_thread_capture);
    m_preprocessor->moveToThread(&s_thread_preprocessor);
    m_converter->moveToThread(&s_thread_converter);
    m_recorder->moveToThread(&s_thread_recorder);


    // Start the framerate update timer
    s_frame_timer.start(FRAMERATE_UPDATE_INTERVAL, this);

    // Connect image pipeline
    connect(m_capture.get(), &Capture::frame_ready, m_preprocessor.get(), &Preprocessor::preprocess_frame);
    connect(m_preprocessor.get(), &Preprocessor::frame_processed, m_converter.get(), &Converter::process_frame);
    connect(m_preprocessor.get(), &Preprocessor::frame_processed, m_recorder.get(), &Recorder::frame_received);
    connect(m_converter.get(), &Converter::image_ready, this, &ImageViewer::set_image);

    // Connect UI signals
    connect(parent, &CameraDisplay::display_opened, m_capture.get(), &Capture::start_capture);
    connect(parent, &CameraDisplay::display_closed, m_capture.get(), &Capture::stop_capture);
    connect(parent, &CameraDisplay::camera_changed, m_capture.get(), &Capture::change_camera);
    connect(parent, &CameraDisplay::effect_changed, m_preprocessor.get(), &Preprocessor::use_modifier);
    connect(parent, &CameraDisplay::zoom_changed, m_preprocessor.get(), &Preprocessor::zoom_changed);
    connect(parent, &CameraDisplay::rotation_changed, m_preprocessor.get(), &Preprocessor::rotation_changed);
    connect(parent, &CameraDisplay::toggle_rotation, this, &ImageViewer::toggle_rotation);
    connect(parent, &CameraDisplay::save_screenshot, this, &ImageViewer::save_screenshot);
    connect(parent, &CameraDisplay::toggle_record, this, &ImageViewer::handle_recording);
    connect(parent, &CameraDisplay::toggle_path, this, &ImageViewer::toggle_path);
    connect(parent, &CameraDisplay::clear_path, this, &ImageViewer::clear_path);
    connect(parent, &CameraDisplay::zoom_changed, this, &ImageViewer::set_zoom);
    connect(parent, &CameraDisplay::set_grid_path, this, &ImageViewer::set_grid_path);
    connect(parent, &CameraDisplay::show_grid, m_grid_display.get(), &GridDisplay::show_grid);
    connect(parent, &CameraDisplay::hide_grid, m_grid_display.get(), &GridDisplay::hide_grid);
    connect(parent, &CameraDisplay::clear_grid, m_grid_display.get(), &GridDisplay::clear_selection);
    connect(parent, &CameraDisplay::select_position, m_grid_display.get(), &GridDisplay::select_robot_position);
    connect(parent, &CameraDisplay::move_grid, m_grid_display.get(), &GridDisplay::update_grid_location);
    connect(this, &ImageViewer::increment_rotation, parent, &CameraDisplay::increment_rotation);
    connect(this, &ImageViewer::start_recording, m_recorder.get(), &Recorder::start_recording);
    connect(this, &ImageViewer::stop_recording, m_recorder.get(), &Recorder::stop_recording);
}

ImageViewer::~ImageViewer() {
    delete ui;
}

void ImageViewer::add_path_point(double pixel_x, double pixel_y) {
    double combined_scale =
        m_preprocessor->get_zoom_factor() *
        m_converter->get_previous_scale();
    double path_x = pixel_x / combined_scale;
    double path_y = pixel_y / combined_scale;
    Main::get()->state().append_path(path_x, path_y);
}

void ImageViewer::set_image(const QImage &img) {
    // Upon first frame capture, resize the widget
    if (m_image.isNull()) { setFixedSize(img.size()); }
    m_image = img;
    // Trigger rerender
    update();
}

void ImageViewer::set_path(const std::vector<vector2i> &pixel_path) {
    log() << "Setting path with " << pixel_path.size() << " nodes";
    CompetitionState &state = Main::get()->state();
    double combined_scale =
        m_preprocessor->get_zoom_factor() *
        m_converter->get_previous_scale();
    double inv_scale = 1.0 / combined_scale;
    double path_x;
    double path_y;
    state.clear_path();
    for (const vector2i &p : pixel_path) {
        path_x = p.x() * inv_scale;
        path_y = p.y() * inv_scale;
        state.append_path(path_x, path_y);
    }
}

void ImageViewer::mousePressEvent(QMouseEvent *ev) {

    grabMouse();
    if (m_selecting_path) {
        add_path_point(ev->x(), ev->y());
    }
    if (m_grid_display->is_displayed()) {
        QRect geometry = m_grid_display->view_geometry();
        m_grid_display->set_mouse_start(QPoint((ev->x() - geometry.x()), (ev->y() - geometry.y())));
        m_grid_display->mousePressEvent(ev);
    }
    QWidget::mousePressEvent(ev);
}

void ImageViewer::mouseReleaseEvent(QMouseEvent *ev) {
    releaseMouse();
    if (m_grid_display->is_displayed()) {
        QRect geometry = m_grid_display->view_geometry();
        m_grid_display->set_mouse_release(QPoint((ev->x() - geometry.x()), (ev->y() - geometry.y())));
        m_grid_display->mouseReleaseEvent(ev);
    }
    QWidget::mouseReleaseEvent(ev);
}

void ImageViewer::mouseMoveEvent(QMouseEvent *ev) {
    if (m_grid_display->is_displayed()) {
        QRect geometry = m_grid_display->view_geometry();
        m_grid_display->set_mouse_move(QPoint((ev->x() - geometry.x()), (ev->y() - geometry.y())));
        m_grid_display->mouseMoveEvent(ev);
    }
    QWidget::mouseMoveEvent(ev);
}


void ImageViewer::timerEvent(QTimerEvent *ev) {
    if (ev->timerId() == s_frame_timer.timerId()) {
        int frames = m_converter->get_and_reset_frames();
        double fps = 1000.0 * frames / FRAMERATE_UPDATE_INTERVAL;
        set_frame_rate(fps);
    } else if (ev->timerId() == s_rotation_timer.timerId()) {
        Q_EMIT increment_rotation();
    }
}

void ImageViewer::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    // Draw the image first
    painter.drawImage(0, 0, m_image);
    painter.setRenderHint(QPainter::Antialiasing);
    // Need to rescale the path nodes
    double combined_scale =
        m_preprocessor->get_zoom_factor() *
        m_converter->get_previous_scale();
    const path2d &path = Main::get()->state().get_path();
    for (std::size_t i = 0; i < path.size(); ++i) {
        // Draw each node and connect lines between them
        QColor color;
        // Color based on start and end
        if (i == 0) { color = Qt::red; }
        else if (i + 1 == path.size()) { color = Qt::blue; }
        else { color = Qt::green; }
        vector2i v1 = path[i] * combined_scale;
        painter.setBrush(color);
        painter.setPen(color);
        painter.drawEllipse(v1.x() - 4, v1.y() - 4, 8, 8);
        if (i > 0) {
            vector2i v0 = path[i - 1] * combined_scale;
            painter.setPen(QPen(Qt::green, 2, Qt::DashDotLine, Qt::RoundCap));
            painter.drawLine(v0.x(), v0.y(), v1.x(), v1.y());
        }
    }
    painter.end();
}

void ImageViewer::set_frame_rate(double frame_rate) {
    ui->fps_label->setText(color_format(frame_rate));
}

void ImageViewer::set_zoom(double zoom) {
    ui->zoom_label->setText(color_format(zoom, "x"));
    m_preprocessor->zoom_changed(zoom);
}

void ImageViewer::save_screenshot(const QString &file) {
    m_image.save(file);
}

void ImageViewer::handle_recording() {
    if (m_recorder->is_recording()) {
        // Stop recording
        Q_EMIT stop_recording();
    } else {
        // Grab the video save path and start recording
        QString file = QFileDialog::getSaveFileName(this, "Save Video", QDir::currentPath(), "Videos (*.avi)");
        log() << "Saving video to: " << file;
        Q_EMIT start_recording(file, m_capture->capture_width(), m_capture->capture_height());
    }
}

void ImageViewer::toggle_path(bool toggle_path) {
    m_selecting_path = toggle_path;
}

void ImageViewer::set_grid_path() {
#ifndef NDEBUG
    assert(g_pm != nullptr);
#endif
    nrg::connect_path(m_grid_display.get(), g_pm);
}

void ImageViewer::clear_path() {
    Main::get()->state().clear_path();
}

void ImageViewer::toggle_rotation(bool rotate) {
    if (rotate) {
        s_rotation_timer.start(ROTATE_UPDATE_INTERVAL, this);
    } else {
        s_rotation_timer.stop();
    }
}

