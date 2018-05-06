#include "parameterslot.h"
#include "ui_parameterslot.h"

#include "parameterbox.h"

ParameterSlot::ParameterSlot(
    const QString &name,
    const QVariant &def,
    ParameterBox *parent
) :
    QWidget(parent),
    ui(new Ui::ParameterSlot) {
    ui->setupUi(this);

    ui->label->setText(name);
    ui->edit->setText(def.toString());
}

#include <QDebug>

void ParameterSlot::set_value() {
    Q_EMIT value_set(QVariant::fromValue(ui->edit->text()));
}

ParameterSlot::~ParameterSlot() {
    delete ui;
}
