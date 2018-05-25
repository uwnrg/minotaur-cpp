#include "serialbox.h"
#include "ui_serialbox.h"

#include "../utility/utility.h"
#include "../utility/font.h"

#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>

SerialBox::SerialBox(
    const std::shared_ptr<Solenoid> &solenoid,
    QWidget *parent
) :
    QDialog(parent),
    ui(new Ui::SerialBox),
    m_solenoid(solenoid),
    m_status(SerialStatus::DISCONNECTED) {
    ui->setupUi(this);
    ui->baud_combo_box->setEditable(false);

    // Set console font of serial display
    ui->serial_display->setFont(qfont::courier(8));
    ui->serial_display->setReadOnly(true);

    // Populate combo boxes
    auto baud_rates = {
        QSerialPort::Baud1200,
        QSerialPort::Baud2400,
        QSerialPort::Baud4800,
        QSerialPort::Baud9600,
        QSerialPort::Baud19200,
        QSerialPort::Baud38400,
        QSerialPort::Baud57600,
        QSerialPort::Baud115200
    };
    for (auto baud_rate : baud_rates) {
        ui->baud_combo_box->addItem(QString::number(baud_rate), QVariant(baud_rate));
    }
    int index = ui->baud_combo_box->findData(QVariant(QSerialPort::Baud9600));
    ui->baud_combo_box->setCurrentIndex(index);

    // Serial ports
    auto serial_ports = QSerialPortInfo::availablePorts();
    for (auto &serial_port : serial_ports) {
        ui->serial_combo_box->addItem(serial_port.portName());
    }

    // Set status
    if (m_solenoid->serial_port().isOpen()) {
        m_status = SerialStatus::CONNECTED;
    }
    update_status(m_status);

    // Setup sliders
    setup_power_slider(*(ui->up_slider));
    setup_power_slider(*(ui->down_slider));
    setup_power_slider(*(ui->left_slider));
    setup_power_slider(*(ui->right_slider));
    setup_slider(*(ui->delay_slider), Delay::DELAY_DEFAULT, Delay::DELAY_MIN, Delay::DELAY_MAX, Delay::DELAY_INTERVAL);
    m_edit_boxes[0] = ui->up_box;
    m_edit_boxes[1] = ui->down_box;
    m_edit_boxes[2] = ui->left_box;
    m_edit_boxes[3] = ui->right_box;
    m_sliders[0] = ui->up_slider;
    m_sliders[1] = ui->down_slider;
    m_sliders[2] = ui->left_slider;
    m_sliders[3] = ui->right_slider;

    QDialogButtonBox *button_box = ui->button_box;

    QPushButton *close_button = button_box->button(QDialogButtonBox::Close);
    close_button->setAutoDefault(false);
    close_button->setDefault(false);

    connect(m_solenoid.get(), &Solenoid::serial_status, this, &SerialBox::update_status);
    connect(ui->disconnect_button, &QPushButton::clicked, m_solenoid.get(), &Solenoid::attempt_disconnect);
    connect(ui->connect_button, &QPushButton::clicked, this, &SerialBox::attempt_connection);
    connect(ui->up_slider, &QSlider::valueChanged, this, &SerialBox::up_slider_changed);
    connect(ui->down_slider, &QSlider::valueChanged, this, &SerialBox::down_slider_changed);
    connect(ui->left_slider, &QSlider::valueChanged, this, &SerialBox::left_slider_changed);
    connect(ui->right_slider, &QSlider::valueChanged, this, &SerialBox::right_slider_changed);
    connect(ui->up_box, &QLineEdit::editingFinished, this, &SerialBox::up_box_changed);
    connect(ui->down_box, &QLineEdit::editingFinished, this, &SerialBox::down_box_changed);
    connect(ui->left_box, &QLineEdit::editingFinished, this, &SerialBox::left_box_changed);
    connect(ui->right_box, &QLineEdit::editingFinished, this, &SerialBox::right_box_changed);
    connect(ui->delay_slider, &QSlider::valueChanged, m_solenoid.get(), &Solenoid::change_delay);

    // Close SerialBox when Cancel is clicked
    connect(ui->button_box->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &QDialog::close);
}

SerialBox::~SerialBox() {
    delete ui;
}

void SerialBox::slider_changed(int value, int dir) {
    m_edit_boxes[dir]->setText(QString::number(value));
    switch (dir) {
        case 0:
            m_solenoid->change_up_power(value);
            break;
        case 1:
            m_solenoid->change_down_power(value);
            break;
        case 2:
            m_solenoid->change_left_power(value);
            break;
        case 3:
            m_solenoid->change_right_power(value);
            break;
    }
}

void SerialBox::up_slider_changed(int value) {
    slider_changed(value, 0);
}

void SerialBox::down_slider_changed(int value) {
    slider_changed(value, 1);
}

void SerialBox::left_slider_changed(int value) {
    slider_changed(value, 2);
}

void SerialBox::right_slider_changed(int value) {
    slider_changed(value, 3);
}

void SerialBox::box_changed(int value, int dir) {
    m_sliders[dir]->setValue(value);
}

void SerialBox::up_box_changed() {
    QString value = ui->up_box->text();
    int power = value.toInt(nullptr, 10);
    if(power >= Power::POWER_MIN && power <= Power::POWER_MAX) {
        box_changed(power, 0);
    }
}

void SerialBox::down_box_changed() {
    QString value = ui->up_box->text();
    int power = value.toInt(nullptr, 10);
    if(power >= Power::POWER_MIN && power <= Power::POWER_MAX) {
        box_changed(power, 1);
    }
}

void SerialBox::left_box_changed() {
    QString value = ui->up_box->text();
    int power = value.toInt(nullptr, 10);
    if(power >= Power::POWER_MIN && power <= Power::POWER_MAX) {
        box_changed(power, 2);
    }
}

void SerialBox::right_box_changed() {
    QString value = ui->up_box->text();
    int power = value.toInt(nullptr, 10);
    if(power >= Power::POWER_MIN && power <= Power::POWER_MAX) {
        box_changed(power, 3);
    }
}

void SerialBox::attempt_connection() {
    QString serial_port = ui->serial_combo_box->currentText();
    if (serial_port.isEmpty()) { return; }
    auto baud_rate = static_cast<QSerialPort::BaudRate>(
        ui->baud_combo_box->currentData().toInt()
    );
    m_solenoid->attempt_connection(serial_port, baud_rate);
}

void SerialBox::update_status(SerialStatus new_status) {
    m_status = new_status;
    QPalette palette = ui->serial_status->palette();
    Qt::GlobalColor color;
    switch (m_status) {
        case SerialStatus::CONNECTED:
            ui->serial_status->setText("CONNECTED");
            color = Qt::blue;
            break;
        case SerialStatus::DISCONNECTED:
            ui->serial_status->setText("DISCONNECTED");
            color = Qt::red;
            break;
        default:
            ui->serial_status->setText("CONNECTING");
            color = Qt::yellow;
            break;
    }
    palette.setColor(ui->serial_status->foregroundRole(), color);
    ui->serial_status->setPalette(palette);
}

void SerialBox::append_text(const std::string &text) {
    auto display = ui->serial_display;
    display->moveCursor(QTextCursor::End);
    display->insertPlainText(QString::fromStdString(text));
    display->moveCursor(QTextCursor::End);
}

void SerialBox::setup_power_slider(QSlider &slider) {
    setup_slider(slider, 255, Power::POWER_MIN, Power::POWER_MAX, Power::POWER_INTERVAL);
}

void SerialBox::setup_slider(QSlider &slider, int value, int min, int max, int interval) {
    slider.setTickInterval(interval);
    slider.setTickPosition(QSlider::TicksBothSides);
    slider.setMaximum(max);
    slider.setMinimum(min);
    slider.setTracking(false);
    slider.setValue(value);
}