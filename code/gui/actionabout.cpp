#include "actionabout.h"
#include "ui_actionabout.h"

ActionAbout::ActionAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ActionAbout)
{
    ui->setupUi(this);

    // make sure github link links to a website
    ui->textBrowser->setOpenExternalLinks(true);

    setWindowTitle(tr("About"));
}

ActionAbout::~ActionAbout()
{
    delete ui;
}
