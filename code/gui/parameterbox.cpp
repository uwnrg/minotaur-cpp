#include "parameterbox.h"
#include "ui_parameterbox.h"
#include "ui_parameterslot.h"

#include "../utility/utility.h"

ParameterSlot::ParameterSlot(ParameterBox *parent, QString &&name) :
    QWidget(parent),
    ui(std::make_unique<Ui::ParameterSlot>()) {}

ParameterBox::ParameterBox(QWidget *parent) :
    QDialog(parent),
    ui(std::make_unique<Ui::ParameterBox>()) {}
