#include "serialbox.h"

#include "../utility/utility.h"
#include "ui_serialbox.h"

#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>

SerialBox::SerialBox(
    const std::shared_ptr<Solenoid> &solenoid,
    QWidget *parent
) :
    QDialog(parent),
    ui(std::make_unique<Ui::SerialBox>()),
    m_solenoid(solenoid),
    m_status(SerialStatus::DISCONNECTED) {
    ui->setupUi(this);
    ui->baud_combo_box->setEditable(false);

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
    connect(m_solenoid.get(), &Solenoid::serial_status, this, &SerialBox::update_status);
    connect(ui->disconnect_button, &QPushButton::clicked, m_solenoid.get(), &Solenoid::attempt_disconnect);
    connect(ui->connect_button, &QPushButton::clicked, this, &SerialBox::attempt_connection);

    // Close SerialBox when Cancel is clicked
    connect(ui->button_box->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &QDialog::close);
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
