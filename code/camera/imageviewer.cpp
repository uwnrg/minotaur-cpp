#include "imageviewer.h"
#include "ui_imageviewer.h"

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

ImageViewer::ImageViewer(CameraDisplay *parent, int fps_update_interval) :
    QWidget(parent),

    ui(new Ui::ImageViewer),

    m_capture(this),
    m_preprocessor(this),
    m_converter(this),
    m_recorder(this),

    m_fps_update_interval(fps_update_interval) {

    ui->setupUi(this);

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
}

ImageViewer::~ImageViewer() {
    delete ui;
}

const QImage &ImageViewer::get_image() {
    return m_image;
}

void ImageViewer::set_image(const QImage &img) {
    m_image = img;
    if (m_image.size() != size()) {
        setFixedSize(m_image.size());
    }
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
    ui->fps_label->setText(QString::number(frame_rate));
}

void ImageViewer::set_zoom(double zoom) {
    ui->zoom_label->setText(QString::number(zoom) + "x");
    m_preprocessor.zoom_changed(zoom);
}
