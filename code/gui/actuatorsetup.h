#ifndef ACTUATORSETUP_H
#define ACTUATORSETUP_H

#include <QDialog>

namespace Ui {
class ActuatorSetup;
}

class ActuatorSetup : public QDialog
{
    Q_OBJECT

public:
    explicit ActuatorSetup(QWidget *parent = 0);
    ~ActuatorSetup();

private:
    Ui::ActuatorSetup *ui;
};

#endif // ACTUATORSETUP_H
