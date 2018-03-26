#include <QCameraInfo>
#include <QFileDialog>
#include <QComboBox>
#include <QStandardItemModel>
#include <QSpinBox>

#include "cameradisplay.h"
#include "ui_cameradisplay.h"

#include "../video/modify.h"
#include "../utility/logger.h"
#include "../utility/utility.h"
#include "../simulator/fakecamera.h"
#include "../gui/griddisplay.h"

#define MAX_GRID_WEIGHT 10

void populate_camera_box(QComboBox *box) {
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    log() << "Found " << cameras.size() << " cameras";
    int i;
    for (i = 0; i < cameras.size(); ++i) {
        box->addItem(cameras[i].deviceName(), QVariant::fromValue(i));
    }
    box->addItem("Simulated", QVariant::fromValue(i));
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
    m_ui(new Ui::CameraDisplay),

    m_action_box(std::make_unique<ActionBox>(this)),
    m_image_viewer(std::make_unique<ImageViewer>(this)) {

    m_ui->setupUi(this);

    // Populate camera and effect lists
    populate_camera_box(m_ui->camera_box);
    populate_effect_box(m_ui->effect_box);

    // Setup zoom slider
    m_ui->zoom_slider->setTickInterval(2);
    m_ui->zoom_slider->setTickPosition(QSlider::TicksBelow);
    m_ui->zoom_slider->setMaximum(40);
    m_ui->zoom_slider->setMinimum(10);

    // Setup rotation slider
    m_ui->rotate_slider->setTickInterval(45);
    m_ui->rotate_slider->setTickPosition(QSlider::TicksBelow);
    m_ui->rotate_slider->setMaximum(180);
    m_ui->rotate_slider->setMinimum(-180);
    // Setup weight selectors
    m_ui->weight_selector->setRange(-1, MAX_GRID_WEIGHT);

    // Setup image viewer
    m_ui->layout->addWidget(m_image_viewer.get());

    // Connections from UI
    connect(m_ui->camera_box, SIGNAL(currentIndexChanged(int)), this, SLOT(camera_box_changed(int)));
    connect(m_ui->effect_box, SIGNAL(currentIndexChanged(int)), this, SLOT(effect_box_changed(int)));
    connect(m_ui->picture_button, &QPushButton::clicked, this, &CameraDisplay::take_screen_shot);
    connect(m_ui->record_button, &QPushButton::clicked, this, &CameraDisplay::toggle_record);
    connect(m_ui->show_grid_button, &QPushButton::clicked, this, &CameraDisplay::show_grid);
    connect(m_ui->clear_grid_button, &QPushButton::clicked, this, &CameraDisplay::clear_grid);
    connect(m_ui->toggle_path_button, &QPushButton::clicked, this, &CameraDisplay::toggle_path);
    connect(m_ui->clear_path_button, &QPushButton::clicked, this, &CameraDisplay::clear_path);
    connect(m_ui->play_button, &QPushButton::clicked, this, &CameraDisplay::pressed_play);
    connect(m_ui->zoom_slider, &QSlider::valueChanged, this, &CameraDisplay::update_zoom);
    connect(m_ui->rotate_slider, &QSlider::valueChanged, this, &CameraDisplay::rotation_slider_changed);
    connect(m_ui->rotation_box, &QLineEdit::editingFinished, this, &CameraDisplay::rotation_box_changed);
    connect(m_ui->weight_list, SIGNAL(currentIndexChanged(int)), this, SLOT(grid_select_changed(int)));
    connect(m_ui->weight_selector, SIGNAL(valueChanged), this, SLOT(weighting_changed(int)));
}

CameraDisplay::~CameraDisplay() {
    delete m_ui;
}

void CameraDisplay::setVisible(bool visible) {
    if (!visible) {
        Q_EMIT display_closed();
        QDialog::setVisible(false);
        return;
    }
    int camera_index = FakeCamera::FAKE_CAMERA;
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
#ifndef NDEBUG
    qDebug() << "Switching to camera index: " << camera;
#endif
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    if (camera < cameras.size()) {
        QCameraInfo info = cameras[camera];
        int camera_index = get_camera_index(info);
        Q_EMIT camera_changed(camera_index);
    } else {
        Q_EMIT camera_changed(FakeCamera::FAKE_CAMERA);
    }
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

void CameraDisplay::update_zoom(int value) {
    double zoom_factor = value / 10.0;
    Q_EMIT zoom_changed(zoom_factor);
}

void CameraDisplay::grid_select_changed(int weight_index) {
    m_selected_weight = m_ui->weight_list->currentText();
       Q_EMIT select_position(m_selected_weight);
#ifndef NDEBUG
    qDebug() << "Grid selector option changed to " << m_selected_weight;
#endif
}

void CameraDisplay::weighting_changed(int weight) {
    m_weighting = weight;
}

int CameraDisplay::get_weighting() {
    return m_weighting;
}

void CameraDisplay::show_grid_clicked() {
    Q_EMIT show_grid();
}

void CameraDisplay::clear_grid_clicked() {
    Q_EMIT clear_grid();
}

void CameraDisplay::rotation_slider_changed(int value) {
    m_ui->rotation_box->setText(QString::number(value));
    Q_EMIT rotation_changed(value);
}

void CameraDisplay::rotation_box_changed() {
    QString value = m_ui->rotation_box->text();
    int degrees = value.toInt(nullptr, 10);
    m_ui->rotate_slider->setValue(degrees);
}

void CameraDisplay::set_rotation(int value) {
    if (value >= -180 && value <= 180) {
        m_ui->rotation_box->setText(QString::number(value));
        m_ui->rotate_slider->setValue(value);
    }
}

void CameraDisplay::increment_rotation() {
    double angle = m_ui->rotate_slider->value();
    angle += 180;
    angle += 1;
    angle = static_cast<int>(angle) % 360;    
    angle -= 180;
    set_rotation(static_cast<int>(angle));
}

void CameraDisplay::pressed_play() {
    Q_EMIT toggle_rotation();
    if (m_ui->play_button->text() == "▶") {
        m_ui->play_button->setText("⏸");
    }
    else {
        m_ui->play_button->setText("▶");
    }
}
