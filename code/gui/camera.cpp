#include "camera.h"
#include "../utility/logger.h"

#include <QCamera>
#include <QPainter>
#include <QCameraInfo>
#include <QAction>
#include <QDir>
#include <QFileDialog>

Capture::Capture(QObject *parent)
    : QObject(parent) {}

int Capture::capture_width() {
    return static_cast<int>(m_video_capture->get(CV_CAP_PROP_FRAME_WIDTH));
}

int Capture::capture_height() {
    return static_cast<int>(m_video_capture->get(CV_CAP_PROP_FRAME_HEIGHT));
}

void Capture::start(int cam) {
    if (cam < 0) {
        return;
    }
    if (!m_video_capture || !m_video_capture->isOpened()) {
        m_video_capture.reset(new cv::VideoCapture(cam));
    }
    if (m_video_capture->isOpened()) {
        m_timer.start(0, this);
        Q_EMIT started();
    }
}

void Capture::stop() {
    m_timer.stop();
    m_video_capture->release();
}

void Capture::timerEvent(QTimerEvent *ev) {
    if (ev->timerId() != m_timer.timerId()) {
        return;
    }
    cv::Mat frame;
    if (!m_video_capture->read(frame)) {
        m_timer.stop();
        return;
    }
    Q_EMIT matReady(frame);
}

Converter::Converter(QObject *parent, CameraDisplay *display)
    : QObject(parent),
      m_display(display) {}

void Converter::setProcessAll(bool process_all) {
    m_process_all = process_all;
}

void Converter::processFrame(const cv::Mat &frame) {
    if (m_process_all) {
        process(frame);
    } else {
        queue(frame);
    }
}

void Converter::startRecording(QString file, int width, int height) {
    constexpr int fps = 30;
    m_recorder.reset(new Recorder(
        file.toStdString(),
        CV_FOURCC('M', 'J', 'P', 'G'),
        fps,
        cv::Size(width, height),
        true)
    );
    connect(this, SIGNAL(frameProcessed(cv::Mat & )), m_recorder.get(), SLOT(image_received(cv::Mat & )));
}

void Converter::stopRecording() {
    if (m_recorder) {
        if (m_recorder->is_recording()) {
            m_recorder->stop_recording();
        }
        m_recorder.reset();
    }
}

bool Converter::is_recording() {
    return m_recorder && m_recorder->is_recording();
}

void Converter::modifierChanged(int modifier_index) {
    VideoModifier::attachModifier(m_modifier, modifier_index);
}

void Converter::matDelete(void *mat) {
    delete static_cast<cv::Mat *>(mat);
}

void Converter::queue(const cv::Mat &frame) {
    m_frame = frame;
    if (!m_timer.isActive()) {
        m_timer.start(0, this);
    }
}

void Converter::process(cv::Mat frame) {
    double scale = MIN(
        (m_display->width() - 20) / (double) frame.size().width,
        (m_display->height() - 20) / (double) frame.size().height
    );
    if (m_modifier) {
        m_modifier->modify(frame);
    }
    if (m_recorder && m_recorder->is_recording()) {
        // If the recorder is active, forward the image
        // to the recorder to write
        Q_EMIT frameProcessed(frame);
    }
    cv::resize(frame, frame, cv::Size(), scale, scale, cv::INTER_AREA);
    cv::cvtColor(frame, frame, CV_BGR2RGB);
    const QImage image(
        frame.data, frame.cols, frame.rows, static_cast<int>(frame.step),
        QImage::Format_RGB888, &matDelete, new cv::Mat(frame)
    );
    Q_ASSERT(image.constBits() == frame.data);
    Q_EMIT imageReady(image);
}

void Converter::imageKeyEvent(int key) {
#ifndef NDEBUG
    qDebug() << "Key pressed: " << key;
#endif
    if (m_modifier) {
        m_modifier->forwardKeyEvent(key);
    }
}

void Converter::timerEvent(QTimerEvent *ev) {
    if (ev->timerId() != m_timer.timerId()) {
        return;
    }
    process(m_frame);
    m_frame.release();
    m_timer.stop();
}

ImageViewer::ImageViewer(QWidget *parent)
    : QWidget(parent) {
    setAttribute(Qt::WA_OpaquePaintEvent);
}

const QImage &ImageViewer::getImage() {
    return m_img;
}

void ImageViewer::setImage(const QImage &img) {
    m_img = img;
    if (m_img.size() != size()) {
        setFixedSize(m_img.size());
    }
    update();
}

void ImageViewer::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(0, 0, m_img);
}

IThread::~IThread() {
    quit();
    wait();
}

CameraDisplay::CameraDisplay(QWidget *parent, int camera_index)
    : QDialog(parent),
      m_camera(camera_index),
      m_converter(nullptr, this) {
    m_layout = new QVBoxLayout(this);
    m_image_viewer = new ImageViewer(this);

    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
#ifndef NDEBUG
    qDebug() << "Found " << QCameraInfo::availableCameras().count() << "cameras" << endl;
#endif
    m_camera_list = new QComboBox(this);
    m_camera_list->setMinimumSize(150, 30);
    for (int i = 0; i < cameras.size(); ++i) {
        m_camera_list->addItem(cameras[i].deviceName(), QVariant::fromValue(i));
    }

    m_effects_list = new QComboBox(this);
    m_effects_list->setMinimumSize(150, 30);
    VideoModifier::addModifierList(m_effects_list);

    m_converter.setProcessAll(false);
    m_capture_thread.start();
    m_converter_thread.start();
    m_capture.moveToThread(&m_capture_thread);
    m_converter.moveToThread(&m_converter_thread);

    m_capture_btn = new QPushButton(this);
    m_capture_btn->setText("Take Picture");

    m_record_btn = new QPushButton(this);
    m_record_btn->setText("Record Video");

    setLayout(m_layout);
    m_layout->addWidget(m_capture_btn);
    m_layout->addWidget(m_record_btn);
    m_layout->addWidget(m_camera_list);
    m_layout->addWidget(m_effects_list);
    m_layout->addWidget(m_image_viewer);

    QObject::connect(&m_capture, &Capture::matReady, &m_converter, &Converter::processFrame);
    QObject::connect(&m_converter, &Converter::imageReady, m_image_viewer, &ImageViewer::setImage);
    QObject::connect(this, &CameraDisplay::forwardKeyEvent, &m_converter, &Converter::imageKeyEvent);

    connect(m_camera_list, SIGNAL(currentIndexChanged(int)), this, SLOT(selectedCameraChanged(int)));
    connect(m_capture_btn, SIGNAL(clicked()), this, SLOT(captureAndSave()));
    connect(m_effects_list, SIGNAL(currentIndexChanged(int)), this, SLOT(effectsChanged(int)));
    connect(m_record_btn, SIGNAL(clicked()), this, SLOT(recordButtonClicked()));
    connect(this, SIGNAL(beginRecording()), this, SLOT(recordSaveFile()));
    connect(this, SIGNAL(recordFileAcquired(QString, int, int)), &m_converter, SLOT(startRecording(QString, int, int)));
    connect(this, SIGNAL(stopRecording()), &m_converter, SLOT(stopRecording()));

    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
}

CameraDisplay::~CameraDisplay() {
    delete m_image_viewer;
    delete m_camera_list;
    delete m_layout;
    delete m_capture_btn;
    delete m_record_btn;
}

void CameraDisplay::setCamera(int camera) {
    m_camera = camera;
}

int CameraDisplay::getCamera() {
    return m_camera;
}

int getCameraIndex(const QCameraInfo &info) {
    QString name = info.deviceName();
    QChar index_char = name.at(name.length() - 1);
    int index = index_char.unicode() - '0';
    return index > 9 ? 0 : index;
}

void CameraDisplay::setVisible(bool visible) {
    if (visible) {
        int camera_index = 0;
        if (!QCameraInfo::availableCameras().empty()) {
            camera_index = getCameraIndex(QCameraInfo::availableCameras()[0]);
        }
        QMetaObject::invokeMethod(&m_capture, "start", Q_ARG(int, camera_index));
    } else {
        pauseVideo();
    }
    setMinimumSize(800, 600);
    QDialog::setVisible(visible);
}

void CameraDisplay::keyPressEvent(QKeyEvent *event) {
    Q_EMIT forwardKeyEvent(event->key());
}

void CameraDisplay::reject() {
    pauseVideo();
    QDialog::reject();
}

void CameraDisplay::pauseVideo() {
    QMetaObject::invokeMethod(&m_capture, "stop");
}

void CameraDisplay::selectedCameraChanged(int list_index) {
    QCameraInfo camera = QCameraInfo::availableCameras()[list_index];
    QMetaObject::invokeMethod(&m_capture, "stop");
    QMetaObject::invokeMethod(&m_capture, "start", Q_ARG(int, getCameraIndex(camera)));
}

void CameraDisplay::effectsChanged(int effect_index) {
    QMetaObject::invokeMethod(&m_converter, "modifierChanged", Q_ARG(int, effect_index));
}

void CameraDisplay::captureAndSave() {
    QString file = QDir::currentPath() + QDir::separator() + "image" + QString::number(m_image_count) + ".png";
#ifndef NDEBUG
    qDebug() << "Saving image " << m_image_count;
    qDebug() << "Target: " << file;
#endif
    if (m_image_viewer->getImage().save(file)) {
        ++m_image_count;
#ifndef NDEBUG
        qDebug() << "Image saved";
#endif
    }
#ifndef NDEBUG
    else {
        qDebug() << "Failed to save image";
    }
#endif
}

void CameraDisplay::recordButtonClicked() {
    if (!m_converter.is_recording()) {
        Logger::log("Started recording video");
#ifndef NDEBUG
        qDebug() << "Recording";
#endif
        Q_EMIT beginRecording();
        m_record_btn->setText("Stop Recording");
    } else {
        Logger::log("Stopping recording and saving video");
        Q_EMIT stopRecording();
        m_record_btn->setText("Record Video");
    }
}

void CameraDisplay::recordSaveFile() {
    QString file = QFileDialog::getSaveFileName(this, tr("Save Video"), QDir::currentPath(),
                                                tr("Videos (*.avi)")); //Opens save-file window
#ifndef NDEBUG
    qDebug() << "Saving video " << m_video_count;
    qDebug() << "Target: " << file;
#endif
    Q_EMIT recordFileAcquired(file, m_capture.capture_width(), m_capture.capture_height());
}

