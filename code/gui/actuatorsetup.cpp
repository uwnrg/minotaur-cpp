#include "actuatorsetup.h"
#include "ui_actuatorsetup.h"

ActuatorSetup::ActuatorSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ActuatorSetup)
{
    ui->setupUi(this);
}

ActuatorSetup::~ActuatorSetup()
{
    delete ui;
}
