#include "cameradisplay.h"

#include <QCameraInfo>
#include <QDir>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QLabel>

#include "imageviewer.h"
#include "../utility/util.h"
#include "../utility/logger.h"

CameraDisplay::CameraDisplay(QWidget *parent, int camera_index)
    : QDialog(parent),
      m_layout(std::make_unique<QVBoxLayout>(this)),
      m_camera_list(std::make_unique<QComboBox>(this)),
      m_effects_list(std::make_unique<QComboBox>(this)),
      m_capture_btn(std::make_unique<QPushButton>(this)),
      m_record_btn(std::make_unique<QPushButton>(this)),
      m_image_viewer(std::make_unique<ImageViewer>(this)),
      m_action_box(std::make_unique<ActionBox>(this)),
      m_framerate_label(std::make_unique<QLabel>(this)),
      m_camera(camera_index),
      m_converter(nullptr, this) {

    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
#ifndef NDEBUG
    qDebug() << "Found " << QCameraInfo::availableCameras().count() << "cameras" << endl;
#endif
    m_camera_list->setMinimumSize(150, 30);
    for (int i = 0; i < cameras.size(); ++i) {
        m_camera_list->addItem(cameras[i].deviceName(), QVariant::fromValue(i));
    }

    m_effects_list->setMinimumSize(150, 30);
    VideoModifier::addModifierList(m_effects_list.get());

    m_converter.setProcessAll(false);
    m_capture_thread.start();
    m_converter_thread.start();
    m_capture.moveToThread(&m_capture_thread);
    m_converter.moveToThread(&m_converter_thread);

    // Start framerate timer to update framerate each second
    m_framerate_timer.start(1000, this);

    m_capture_btn->setText(tr("Take Picture"));
    m_record_btn->setText(tr("Record Video"));

    m_framerate_label->setText("0");
    m_framerate_label->setFixedSize(20, 16);
    m_framerate_label->setStyleSheet("QLabel {background: white;}");

    setLayout(m_layout.get());
    m_layout->addWidget(m_capture_btn.get());
    m_layout->addWidget(m_record_btn.get());
    m_layout->addWidget(m_camera_list.get());
    m_layout->addWidget(m_effects_list.get());
    m_layout->addWidget(m_image_viewer.get());
    m_layout->addWidget(m_framerate_label.get());

    // Video capturing and displaying connections
    connect(&m_capture, &Capture::matReady, &m_converter, &Converter::processFrame);
    connect(&m_converter, &Converter::imageReady, m_image_viewer.get(), &ImageViewer::setImage);
    connect(this, &CameraDisplay::forwardKeyEvent, &m_converter, &Converter::imageKeyEvent);

    // Connections for changing the effects
    connect(m_camera_list.get(), SIGNAL(currentIndexChanged(int)), this, SLOT(selectedCameraChanged(int)));
    connect(m_effects_list.get(), SIGNAL(currentIndexChanged(int)), this, SLOT(effectsChanged(int)));
    connect(m_capture_btn.get(), SIGNAL(clicked()), this, SLOT(captureAndSave()));

    // Connections for recording video
    connect(m_record_btn.get(), &QPushButton::clicked, this, &CameraDisplay::recordButtonClicked);
    connect(this, &CameraDisplay::beginRecording, this, &CameraDisplay::recordSaveFile);
    connect(this, &CameraDisplay::recordFileAcquired, &m_converter, &Converter::startRecording);
    connect(this, &CameraDisplay::stopRecording, &m_converter, &Converter::stopRecording);

    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
}

CameraDisplay::~CameraDisplay() = default;

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
    setMinimumSize(800, 800);
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
    QMetaObject::invokeMethod(&m_converter, "modifierChanged",
                              Q_ARG(int, effect_index),
                              Q_ARG(ActionBox *, m_action_box.get()));
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
        qDebug() << "Recording video start";
#endif
        Q_EMIT beginRecording();
        m_record_btn->setText(tr("Stop Recording"));
    } else {
        Logger::log("Finished recording and saving video");
        Q_EMIT stopRecording();
        m_record_btn->setText(tr("Record Video"));
    }
}

void CameraDisplay::recordSaveFile() {
    // Open save-file window to select a file name and location
    QString file = QFileDialog::getSaveFileName(this, tr("Save Video"), QDir::currentPath(), tr("Videos (*.avi)"));
#ifndef NDEBUG
    qDebug() << "Saving video to target: " << file;
#endif
    Q_EMIT recordFileAcquired(file, m_capture.capture_width(), m_capture.capture_height());
}

void CameraDisplay::requestActionButtons(int num_buttons) {
    // Clear buttons, unique_ptr destructor handles cleanup
    m_action_btns.clear();
    m_action_btn_ptrs.clear();
    for (int i = 0; i < num_buttons; ++i) {
        // The unique_ptr in the vector has ownership
        auto *ptr = new ActionButton(m_action_box.get());
        m_action_btns.emplace_back(ptr);
        m_action_btn_ptrs.push_back(ptr);
        m_action_box->add_action(ptr);
    }
    Q_EMIT returnActionButtons(m_action_btn_ptrs, m_action_box.get());
}

void CameraDisplay::update_framerate(int frames) {
    m_framerate_label->setText(QString::number(frames));
}

void CameraDisplay::timerEvent(QTimerEvent *ev) {
    if (ev->timerId() == m_framerate_timer.timerId()) {
        update_framerate(m_converter.get_and_reset_frames());
    }
}
