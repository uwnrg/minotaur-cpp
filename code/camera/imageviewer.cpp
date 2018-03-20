#include "imageviewer.h"
#include "ui_imageviewer.h"
#include "cameradisplay.h"
#include "../utility/logger.h"

#include <QFileDialog>

QString color_format(double value, const QString &suffix = "") {
    return QString("<font color=\"#8ae234\">%1%2</font>").arg(value).arg(suffix);
}

ImageViewer::ImageViewer(CameraDisplay *parent, int fps_update_interval) :
    QWidget(parent),

    ui(new Ui::ImageViewer),

    m_grid_display(std::make_unique<GridDisplay>(this)),

    m_capture(),
    m_preprocessor(),
    m_converter(this),
    m_recorder(),

    m_fps_update_interval(fps_update_interval) {

    ui->setupUi(this);
    ui->zoom_label->lower();
    ui->fps_label->lower();

    setAttribute(Qt::WA_OpaquePaintEvent);

    // Start threads and move image pipeline objects to threads
    m_thread_capture.start();
    m_thread_preprocessor.start();
    m_thread_converter.start();
    m_thread_recorder.start();
    m_capture.moveToThread(&m_thread_capture);
    m_preprocessor.moveToThread(&m_thread_preprocessor);
    m_converter.moveToThread(&m_thread_converter);
    m_recorder.moveToThread(&m_thread_recorder);

    // Start the framerate update timer
    m_frame_timer.start(fps_update_interval, this);

    // Connect image pipeline
    connect(&m_capture, &Capture::frame_ready, &m_preprocessor, &Preprocessor::preprocess_frame);
    connect(&m_preprocessor, &Preprocessor::frame_processed, &m_converter, &Converter::process_frame);
    connect(&m_preprocessor, &Preprocessor::frame_processed, &m_recorder, &Recorder::frame_received);
    connect(&m_converter, &Converter::image_ready, this, &ImageViewer::set_image);

    // Connect UI signals
    connect(parent, &CameraDisplay::display_opened, &m_capture, &Capture::start_capture);
    connect(parent, &CameraDisplay::display_closed, &m_capture, &Capture::stop_capture);
    connect(parent, &CameraDisplay::camera_changed, &m_capture, &Capture::change_camera);
    connect(parent, &CameraDisplay::effect_changed, &m_preprocessor, &Preprocessor::use_modifier);
    connect(parent, &CameraDisplay::zoom_changed, &m_preprocessor, &Preprocessor::zoom_changed);
    connect(parent, &CameraDisplay::save_screenshot, this, &ImageViewer::save_screenshot);
    connect(parent, &CameraDisplay::zoom_changed, this, &ImageViewer::set_zoom);
    connect(parent, &CameraDisplay::toggle_record, this, &ImageViewer::handle_recording);
    connect(parent, &CameraDisplay::show_grid, m_grid_display.get(), &GridDisplay::showGrid);
    connect(parent, &CameraDisplay::clear_grid, m_grid_display.get(), &GridDisplay::clearSelection);
    connect(this, &ImageViewer::start_recording, &m_recorder, &Recorder::start_recording);
    connect(this, &ImageViewer::stop_recording, &m_recorder, &Recorder::stop_recording);
}

ImageViewer::~ImageViewer() {
    delete ui;
}

const QImage &ImageViewer::get_image() {
    return m_image;
}

void ImageViewer::set_image(const QImage &img) {
    // Upon first frame capture, resize the widget
    if (m_image.isNull()) { setFixedSize(img.size()); }
    m_image = img;
    update();
}

void ImageViewer::timerEvent(QTimerEvent *ev) {
    if (ev->timerId() == m_frame_timer.timerId()) {
        int frames = m_converter.get_and_reset_frames();
        double fps = 1000.0 * frames / m_fps_update_interval;
        set_frame_rate(fps);
    }
}

void ImageViewer::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(0, 0, m_image);
}

void ImageViewer::set_frame_rate(double frame_rate) {
    ui->fps_label->setText(color_format(frame_rate));
}

void ImageViewer::set_zoom(double zoom) {
    ui->zoom_label->setText(color_format(zoom, "x"));
    m_preprocessor.zoom_changed(zoom);
}

void ImageViewer::save_screenshot(const QString &file) {
    m_image.save(file);
}

void ImageViewer::handle_recording() {
    if (m_recorder.is_recording()) {
        // Stop recording
        Q_EMIT stop_recording();
    } else {
        // Start recording
        QString file = QFileDialog::getSaveFileName(this, "Save Video", QDir::currentPath(), "Videos (*.avi)");
        log() << "Saving video to: " << file;
        Q_EMIT start_recording(file, m_capture.capture_width(), m_capture.capture_height());
    }
}
