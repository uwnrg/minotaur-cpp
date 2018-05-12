#include <QCameraInfo>
#include <QFileDialog>
#include <QComboBox>
#include <QStandardItemModel>
#include <QSpinBox>

#include "cameradisplay.h"
#include "ui_cameradisplay.h"

#include "../video/modify.h"
#include "../utility/logger.h"
#include "../simulator/fakecamera.h"

void populate_camera_box(QComboBox *box) {
    // Grab the list of selected cameras
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    log() << "Found " << cameras.size() << " cameras";
    int i;
    for (i = 0; i < cameras.size(); ++i) {
        // Push them to the combo box with their index
        box->addItem(cameras[i].deviceName(), QVariant::fromValue(i));
    }
    // Add the simulated camera
    box->addItem("Simulated", QVariant::fromValue(i));
}

void populate_effect_box(QComboBox *box) {
    // Call the video modifier list adder
    VideoModifier::add_modifier_list(box);
}

void ensure_png(QString &file) {
    // Ensure that the file extension ends with png
    if (file.rightRef(4) != ".png") {
        file.append(".png");
    }
}

int get_camera_index(const QCameraInfo &info) {
    // The camera index is the last character value
    // This method is only really reliable on Linux, where the
    // hardware indices are in the name as /dev/vid1, for example
    QString name = info.deviceName();
    QChar index_char = name.at(name.length() - 1);
    int index = index_char.unicode() - '0';
    // If the index is out of range, go to the default camera
    return index > 9 ? 0 : index;
}

CameraDisplay::CameraDisplay(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::CameraDisplay),

    m_action_box(std::make_unique<ActionBox>(this)),
    m_image_viewer(std::make_unique<ImageViewer>(this)),

    m_weighting(0) {

    m_ui->setupUi(this);

    // Populate camera and effect lists
    populate_camera_box(m_ui->camera_box);
    populate_effect_box(m_ui->effect_box);

    // Setup zoom slider
    m_ui->zoom_slider->setTickInterval(Zoom::ZOOM_INTERVAL);
    m_ui->zoom_slider->setTickPosition(QSlider::TicksBelow);
    m_ui->zoom_slider->setMaximum(Zoom::ZOOM_MAX);
    m_ui->zoom_slider->setMinimum(Zoom::ZOOM_MIN);

    // Setup rotation slider
    m_ui->rotate_slider->setTickInterval(Rotation::ROTATION_INTERVAL);
    m_ui->rotate_slider->setTickPosition(QSlider::TicksBelow);
    m_ui->rotate_slider->setMaximum(Rotation::ROTATION_MAX);
    m_ui->rotate_slider->setMinimum(Rotation::ROTATION_MIN);

    // Setup weight selectors
    m_ui->weight_selector->setRange(Grid::MIN_WEIGHT, Grid::MAX_WEIGHT);

    // Setup image viewer
    m_ui->layout->addWidget(m_image_viewer.get());

    // Connections from UI
    connect(m_ui->camera_box, qol<int>::of(&QComboBox::currentIndexChanged), this, &CameraDisplay::camera_box_changed);
    connect(m_ui->effect_box, qol<int>::of(&QComboBox::currentIndexChanged), this, &CameraDisplay::effect_box_changed);
    connect(m_ui->weight_list, qol<int>::of(&QComboBox::currentIndexChanged), this, &CameraDisplay::grid_select_changed);
    connect(m_ui->weight_selector, qol<int>::of(&QSpinBox::valueChanged), this, &CameraDisplay::weighting_changed);
    connect(m_ui->picture_button, &QPushButton::clicked, this, &CameraDisplay::take_screen_shot);
    connect(m_ui->record_button, &QPushButton::clicked, this, &CameraDisplay::toggle_record);
    connect(m_ui->show_grid_button, &QPushButton::clicked, this, &CameraDisplay::show_grid);
    connect(m_ui->hide_grid_button, &QPushButton::clicked, this, &CameraDisplay::hide_grid);
    connect(m_ui->clear_grid_button, &QPushButton::clicked, this, &CameraDisplay::clear_grid);
    connect(m_ui->toggle_path_button, &QPushButton::clicked, this, &CameraDisplay::toggle_path);
    connect(m_ui->clear_path_button, &QPushButton::clicked, this, &CameraDisplay::clear_path);
    connect(m_ui->play_button, &QPushButton::clicked, this, &CameraDisplay::pressed_play);
    connect(m_ui->zoom_slider, &QSlider::valueChanged, this, &CameraDisplay::update_zoom);
    connect(m_ui->rotate_slider, &QSlider::valueChanged, this, &CameraDisplay::rotation_slider_changed);
    connect(m_ui->rotation_box, &QLineEdit::editingFinished, this, &CameraDisplay::rotation_box_changed);
    //    connect(m_ui->clear_path_button, &QPushButton::clicked, this, &GridDisplay::mouse_clicked);
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
    // When opening, grab the camera index and emit
    int camera_index = FakeCamera::FAKE_CAMERA;
    if (!QCameraInfo::availableCameras().empty()) {
        camera_index = get_camera_index(QCameraInfo::availableCameras()[0]);
    }
    QDialog::setVisible(true);
    Q_EMIT display_opened(camera_index);
}

void CameraDisplay::reject() {
    QDialog::reject();
    // Ensure that the image viewer stops when the dialog is rejected
    Q_EMIT display_closed();
}

void CameraDisplay::camera_box_changed(int camera) {
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    if (camera < cameras.size()) {
        QCameraInfo info = cameras[camera];
        int camera_index = get_camera_index(info);
        Q_EMIT camera_changed(camera_index);
    } else {
        // Emit fake camera if the index is out of range
        Q_EMIT camera_changed(FakeCamera::FAKE_CAMERA);
    }
}

void CameraDisplay::effect_box_changed(int effect) {
    // Grab the modifier corresponding to the effect index
    std::shared_ptr<VideoModifier> modifier = VideoModifier::get_modifier(effect);
    m_action_box->reset_actions();
    // Register actions of the modifier
    if (modifier) { modifier->register_actions(m_action_box.get()); }
    // Emit new modifier
    Q_EMIT effect_changed(modifier);
}

void CameraDisplay::take_screen_shot() {
    // Open file dialog so that the user can select the file
    QString image_png = QFileDialog::getSaveFileName(this, "Save Screenshot", QDir::currentPath(), "Photos (*.png)");
    ensure_png(image_png);
    log() << "Saving screenshot: " << image_png;
    Q_EMIT save_screenshot(image_png);
}

void CameraDisplay::update_zoom(int value) {
    // Scale the zoom factor
    double zoom_factor = value / 10.0;
    Q_EMIT zoom_changed(zoom_factor);
}

void CameraDisplay::grid_select_changed(int) {
    Q_EMIT select_position(m_ui->weight_list->currentText());
}

void CameraDisplay::weighting_changed(int weight) {
    m_weighting = weight;
}

int CameraDisplay::get_weighting() {
    return m_weighting;
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
    angle = static_cast<int>(angle + 181) % 360 - 180;
    set_rotation(static_cast<int>(angle));
}

void CameraDisplay::pressed_play(bool checked) {
    Q_EMIT toggle_rotation(checked);
    m_ui->play_button->setText(checked ? "⏸" : "▶");
}
