#include "actuatorsetup.h"
#include "ui_actuatorsetup.h"
#include "../controller/actuator.h"
#include <QtCore>

ActuatorSetup::ActuatorSetup(std::shared_ptr<Actuator> controller, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ActuatorSetup)
{
    ui->setupUi(this);

	//! [0]
	foreach(QextPortInfo info, QextSerialEnumerator::getPorts())
		ui->portBox->addItem(info.portName);
	//make sure user can input their own port name!
	ui->portBox->setEditable(true);

	ui->baudRateBox->addItem("1200", BAUD1200);
	ui->baudRateBox->addItem("2400", BAUD2400);
	ui->baudRateBox->addItem("4800", BAUD4800);
	ui->baudRateBox->addItem("9600", BAUD9600);
	ui->baudRateBox->addItem("19200", BAUD19200);
	ui->baudRateBox->setCurrentIndex(3);

	ui->parityBox->addItem("NONE", PAR_NONE);
	ui->parityBox->addItem("ODD", PAR_ODD);
	ui->parityBox->addItem("EVEN", PAR_EVEN);

	ui->dataBitsBox->addItem("5", DATA_5);
	ui->dataBitsBox->addItem("6", DATA_6);
	ui->dataBitsBox->addItem("7", DATA_7);
	ui->dataBitsBox->addItem("8", DATA_8);
	ui->dataBitsBox->setCurrentIndex(3);

	ui->stopBitsBox->addItem("1", STOP_1);
	ui->stopBitsBox->addItem("2", STOP_2);

	ui->queryModeBox->addItem("Polling", QextSerialPort::Polling);
	ui->queryModeBox->addItem("EventDriven", QextSerialPort::EventDriven);

	m_current_settings = { BAUD9600, DATA_8, PAR_NONE, STOP_1, FLOW_XONXOFF, 10 };
    controller = std::shared_ptr<Actuator>(new Actuator(ui->portBox->currentText(), m_current_settings));
	m_emma = controller;

	m_enumerator = new QextSerialEnumerator(this);
	m_enumerator->setUpNotifications();

	connect(ui->buttonBox, SIGNAL(accepted()), SLOT(OnSettingsApplied()));
	//TODO: Refresh to the current settings if button box is rejected
	connect(ui->portBox, SIGNAL(editTextChanged(QString)), SLOT(OnPortNameChanged(QString)));

	connect(m_enumerator, SIGNAL(deviceDiscovered(QextPortInfo)), SLOT(OnPortAddedOrRemoved()));
	connect(m_enumerator, SIGNAL(deviceRemoved(QextPortInfo)), SLOT(OnPortAddedOrRemoved()));

	Logger::log("Actuator initialized", Logger::INFO);

	setWindowTitle(tr("Actuator Setup"));
}

ActuatorSetup::~ActuatorSetup()
{
    m_emma.reset();
    delete m_enumerator;
    delete ui;
}

void ActuatorSetup::onSettingsApplied(){ 
	PortSettings settings = {
		(BaudRateType)ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt(),
		(DataBitsType)ui->dataBitsBox->itemData(ui->baudRateBox->currentIndex()).toInt(),
		(ParityType)ui->parityBox->itemData(ui->parityBox->currentIndex()).toInt(),
		(StopBitsType)ui->stopBitsBox->itemData(ui->stopBitsBox->currentIndex()).toInt(),
		FLOW_XONXOFF,
		ui->timeoutBox->value()
	};

	if (m_emma->changeSettings(settings) == 0) {
		m_current_settings = settings;
	}
	else {
		//TODO: Spawn an error dialog
	}
}

void ActuatorSetup::onPortNameChanged(const QString & name)
{
	m_emma->setSerPort(name);
}

void ActuatorSetup::onPortAddedOrRemoved()
{
	QString current = ui->portBox->currentText();

	ui->portBox->blockSignals(true);
	ui->portBox->clear();
	foreach(QextPortInfo info, QextSerialEnumerator::getPorts())
		ui->portBox->addItem(info.portName);

	ui->portBox->setCurrentIndex(ui->portBox->findText(current));

	ui->portBox->blockSignals(false);
}
