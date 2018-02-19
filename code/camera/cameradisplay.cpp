#include "cameradisplay.h"

#include <QCameraInfo>
#include <QPushButton>
#include <QDir>
#include <QKeyEvent>

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

    setLayout(m_layout);
    m_layout->addWidget(m_capture_btn);
    m_layout->addWidget(m_camera_list);
    m_layout->addWidget(m_effects_list);
    m_layout->addWidget(m_image_viewer);

    QObject::connect(&m_capture, &Capture::matReady, &m_converter, &Converter::processFrame);
    QObject::connect(&m_converter, &Converter::imageReady, m_image_viewer, &ImageViewer::setImage);
    QObject::connect(this, &CameraDisplay::forwardKeyEvent, &m_converter, &Converter::imageKeyEvent);

    connect(m_camera_list, SIGNAL(currentIndexChanged(int)), this, SLOT(selectedCameraChanged(int)));
    connect(m_capture_btn, SIGNAL(clicked()), this, SLOT(captureAndSave()));
    connect(m_effects_list, SIGNAL(currentIndexChanged(int)), this, SLOT(effectsChanged(int)));

    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
}

CameraDisplay::~CameraDisplay() {
    delete m_image_viewer;
    delete m_camera_list;
    delete m_layout;
    delete m_capture_btn;
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
