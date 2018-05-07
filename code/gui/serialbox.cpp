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
    ui->up_slider->setTickInterval(Power::POWER_INTERVAL);
    ui->up_slider->setTickPosition(QSlider::TicksBothSides);
    ui->up_slider->setMaximum(Power::POWER_MAX);
    ui->up_slider->setMinimum(Power::POWER_MIN);
    ui->up_slider->setTracking(false);
    ui->up_slider->setValue(255);

    ui->down_slider->setTickInterval(Power::POWER_INTERVAL);
    ui->down_slider->setTickPosition(QSlider::TicksBothSides);
    ui->down_slider->setMaximum(Power::POWER_MAX);
    ui->down_slider->setMinimum(Power::POWER_MIN);
    ui->down_slider->setTracking(false);
    ui->down_slider->setValue(255);

    ui->left_slider->setTickInterval(Power::POWER_INTERVAL);
    ui->left_slider->setTickPosition(QSlider::TicksBothSides);
    ui->left_slider->setMaximum(Power::POWER_MAX);
    ui->left_slider->setMinimum(Power::POWER_MIN);
    ui->left_slider->setTracking(false);
    ui->left_slider->setValue(255);

    ui->right_slider->setTickInterval(Power::POWER_INTERVAL);
    ui->right_slider->setTickPosition(QSlider::TicksBothSides);
    ui->right_slider->setMaximum(Power::POWER_MAX);
    ui->right_slider->setMinimum(Power::POWER_MIN);
    ui->right_slider->setTracking(false);
    ui->right_slider->setValue(255);

    connect(m_solenoid.get(), &Solenoid::serial_status, this, &SerialBox::update_status);
    connect(ui->disconnect_button, &QPushButton::clicked, m_solenoid.get(), &Solenoid::attempt_disconnect);
    connect(ui->connect_button, &QPushButton::clicked, this, &SerialBox::attempt_connection);
    connect(ui->up_slider, &QSlider::valueChanged, m_solenoid.get(), &Solenoid::change_up_power);
    connect(ui->down_slider, &QSlider::valueChanged, m_solenoid.get(), &Solenoid::change_down_power);
    connect(ui->left_slider, &QSlider::valueChanged, m_solenoid.get(), &Solenoid::change_left_power);
    connect(ui->right_slider, &QSlider::valueChanged, m_solenoid.get(), &Solenoid::change_right_power);

    // Close SerialBox when Cancel is clicked
    connect(ui->button_box->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &QDialog::close);
}

SerialBox::~SerialBox() {
    delete ui;
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
