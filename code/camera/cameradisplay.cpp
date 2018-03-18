#include <QCameraInfo>
#include <QFileDialog>
#include <QComboBox>

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

void ensure_png(QString &file) {
    if (file.rightRef(4) != ".png") {
        file.append(".png");
    }
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

    m_action_box(std::make_unique<ActionBox>(this)),
    m_image_viewer(std::make_unique<ImageViewer>(this)) {

    ui->setupUi(this);

    // Populate camera and effect lists
    populate_camera_box(ui->camera_box);
    populate_effect_box(ui->effect_box);

    // Setup zoom slider
    ui->zoom_slider->setTickInterval(2);
    ui->zoom_slider->setTickPosition(QSlider::TicksBelow);
    ui->zoom_slider->setMaximum(40);
    ui->zoom_slider->setMinimum(10);

    // Setup rotation slider
    ui->rotate_slider->setTickInterval(45);
    ui->rotate_slider->setTickPosition(QSlider::TicksBelow);
    ui->rotate_slider->setMaximum(180);
    ui->rotate_slider->setMinimum(-180);

    // Setup image viewer
    ui->layout->addWidget(m_image_viewer.get());

    // Connections from UI
    connect(ui->camera_box, SIGNAL(currentIndexChanged(int)), this, SLOT(camera_box_changed(int)));
    connect(ui->effect_box, SIGNAL(currentIndexChanged(int)), this, SLOT(effect_box_changed(int)));
    connect(ui->picture_button, &QPushButton::clicked, this, &CameraDisplay::take_screen_shot);
    connect(ui->record_button, &QPushButton::clicked, this, &CameraDisplay::record_clicked);
    connect(ui->zoom_slider, &QSlider::valueChanged, this, &CameraDisplay::update_zoom);
    connect(ui->rotate_slider, &QSlider::valueChanged, this, &CameraDisplay::rotation_slider_changed);
    connect(ui->rotation_box, &QLineEdit::editingFinished, this, &CameraDisplay::rotation_box_changed);
    connect(ui->play_button, &QPushButton::clicked, this, &CameraDisplay::pressed_play);
}

CameraDisplay::~CameraDisplay() {
    delete ui;
}

void CameraDisplay::setVisible(bool visible) {
    if (!visible) {
        Q_EMIT display_closed();
        QDialog::setVisible(false);
        return;
    }
    int camera_index = 0;
    if (!QCameraInfo::availableCameras().empty()) {
        camera_index = get_camera_index(QCameraInfo::availableCameras()[0]);
    }
    QDialog::setVisible(true);
    Q_EMIT display_opened(camera_index);
}

void CameraDisplay::reject() {
    QDialog::reject();
    Q_EMIT display_closed();
}

void CameraDisplay::camera_box_changed(int camera) {
    QCameraInfo info = QCameraInfo::availableCameras()[camera];
    int camera_index = get_camera_index(info);
    Q_EMIT camera_changed(camera_index);
}

void CameraDisplay::effect_box_changed(int effect) {
    std::shared_ptr<VideoModifier> modifier = VideoModifier::get_modifier(effect);
    m_action_box->reset_actions();
    if (modifier) { modifier->register_actions(m_action_box.get()); }
    Q_EMIT effect_changed(modifier);
}

void CameraDisplay::take_screen_shot() {
    QString image_png = QFileDialog::getSaveFileName(this, "Save Screenshot", QDir::currentPath(), "Photos (*.png)");
    ensure_png(image_png);
    log() << "Saving screenshot: " << image_png;
    Q_EMIT save_screenshot(image_png);
}

void CameraDisplay::record_clicked() {
    Q_EMIT toggle_recording();
}

void CameraDisplay::update_zoom(int value) {
    double zoom_factor = value / 10.0;
    Q_EMIT zoom_changed(zoom_factor);
}

void CameraDisplay::rotation_slider_changed(int value) {
    ui->rotation_box->setText(QString::number(value));
    Q_EMIT rotation_changed(value);
}

void CameraDisplay::rotation_box_changed() {
    QString value = ui->rotation_box->text();
    int degrees = value.toInt(nullptr, 10);
    ui->rotate_slider->setValue(degrees);
}

void CameraDisplay::set_rotation(int value) {
    if (value >= -180 && value <= 180) {
        ui->rotation_box->setText(QString::number(value));
        ui->rotate_slider->setValue(value);
    }
}

void CameraDisplay::pressed_play() {
    Q_EMIT toggle_rotation();
    if (ui->play_button->text() == "▶") {
        ui->play_button->setText("⏸");
    }
    else {
        ui->play_button->setText("▶");
    }
}