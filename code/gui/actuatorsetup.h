#ifndef ACTUATORSETUP_H
#define ACTUATORSETUP_H

#include "qextserialport.h"
#include "qextserialenumerator.h"

#include <QDialog>

class QextSerialEnumerator;
class Actuator;

namespace Ui {
class ActuatorSetup;
}

class ActuatorSetup : public QDialog
{
    Q_OBJECT

public:
    explicit ActuatorSetup(QWidget *parent = 0);
    ~ActuatorSetup();

private Q_SLOTS:
	void OnPortNameChanged(const QString &name);
	void OnSettingsApplied();

	void OnPortAddedOrRemoved();

private:
	Ui::ActuatorSetup *ui;
	QTimer *timer;
	Actuator *m_emma;
	QextSerialEnumerator *m_enumerator;
	PortSettings m_current_settings;
};

#endif // ACTUATORSETUP_H
