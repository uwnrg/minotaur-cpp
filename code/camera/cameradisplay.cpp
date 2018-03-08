#include <QCameraInfo>

#include "cameradisplay.h"
#include "ui_cameradisplay.h"

#include "../video/modify.h"
#include "../utility/logger.h"
#include "../utility/utility.h"

void populate_camera_box(QComboBox *box) {
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    log() << "Found " << cameras.size() << " cameras";
    for (int i = 0; i < cameras.size(); ++i) {
        box->addItem(cameras[i].deviceName(), QVariant::fromValue(i));
    }
}

void populate_effect_box(QComboBox *box) {
    VideoModifier::add_modifier_list(box);
}

int get_camera_index(const QCameraInfo &info) {
    QString name = info.deviceName();
    QChar index_char = name.at(name.length() - 1);
    int index = index_char.unicode() - '0';
    return index > 9 ? 0 : index;
}

CameraDisplay::CameraDisplay(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraDisplay),

    m_action_box(std::make_unique<ActionBox>(this)) {

    ui->setupUi(this);

    // Populate camera and effect lists
    populate_camera_box(ui->camera_box);
    populate_effect_box(ui->effect_box);

    // Setup zoom slider
    ui->zoom_slider->setTickInterval(2);
    ui->zoom_slider->setTickPosition(QSlider::TicksBelow);
    ui->zoom_slider->setMaximum(40);
    ui->zoom_slider->setMinimum(10);

    // Connections from UI
    connect(ui->camera_box, &QComboBox::currentIndexChanged, this, &CameraDisplay::camera_box_changed);
    connect(ui->effect_box, &QComboBox::currentIndexChanged, this, &CameraDisplay::effect_box_changed);
    connect(ui->picture_button, &QPushButton::clicked, this, &CameraDisplay::take_screenshot);
    connect(ui->record_button, &QPushButton::clicked, this, &CameraDisplay::record_clicked);
    connect(ui->zoom_slider, &QSlider::valueChanged, this, &CameraDisplay::update_zoom);
}

CameraDisplay::~CameraDisplay() {
    delete ui;
}

void CameraDisplay::setVisible(bool visible) {
    if (!visible) {
        Q_EMIT display_closed();
        return;
    }
    int camera_index = 0;
    if (!QCameraInfo::availableCameras().empty()) {
        camera_index = get_camera_index(QCameraInfo::availableCameras()[0]);
    }
    QDialog::setVisible(visible);
    Q_EMIT display_opened(camera_index);
}

void CameraDisplay::reject() {
    Q_EMIT display_closed();
    QDialog::reject();
    Q_EMIT display_closed();
}

void CameraDisplay::camera_box_changed(int camera) {
    QCameraInfo info = QCameraInfo::availableCameras()[camera];
    int camera_index = get_camera_index(info);
    Q_EMIT camera_changed(camera_index);
}

void CameraDisplay::effect_box_changed(int effect) {

}

void CameraDisplay::take_screenshot() {

}

void CameraDisplay::record_clicked() {

}

void CameraDisplay::update_zoom(int value) {

}
