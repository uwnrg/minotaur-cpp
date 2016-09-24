#ifndef ACTUATORSETUP_H
#define ACTUATORSETUP_H

#include "qextserialport.h"
#include "qextserialenumerator.h"

#include <QDialog>
#include <memory>

class QextSerialEnumerator;
class Actuator;

namespace Ui {
class ActuatorSetup;
}

class ActuatorSetup : public QDialog
{
    Q_OBJECT

public:
	ActuatorSetup(std::shared_ptr<Actuator> controller, QWidget *parent = 0);
	~ActuatorSetup();

private Q_SLOTS:
	void onPortNameChanged(const QString &name);
	void onSettingsApplied();

	void onPortAddedOrRemoved();

private:
	Ui::ActuatorSetup *ui;
	QTimer *timer;
	std::shared_ptr<Actuator> m_emma;
	QextSerialEnumerator *m_enumerator;
	PortSettings m_current_settings;
};

#endif // ACTUATORSETUP_H
