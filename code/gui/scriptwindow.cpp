#include "scriptwindow.h"
#include "ui_scriptwindow.h"

ScriptWindow::ScriptWindow(std::shared_ptr<ScriptEngine>& script_engine, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScriptWindow),
    m_script_engine(script_engine)
{
    ui->setupUi(this);
}

ScriptWindow::~ScriptWindow()
{
    delete ui;
}
