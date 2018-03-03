#include <QPixmap>
#include "actionabout.h"
#include "ui_actionabout.h"

#include "../utility/utility.h"

ActionAbout::ActionAbout(QWidget *parent) :
    QDialog(parent),
    ui(std::make_unique<Ui::ActionAbout>()) {
    ui->setupUi(this);
    QPixmap pix(":/graphics/img/UWNRG.png");
    ui->graphicsView->setFrameStyle(QFrame::NoFrame);
    ui->textBrowser->setFrameStyle(QFrame::NoFrame);
    ui->textBrowser2->setFrameStyle(QFrame::NoFrame);
    ui->label_pic->setPixmap(pix);
    ui->label_pic->setScaledContents(true);

    // make sure github link links to a website
    ui->textBrowser2->setOpenExternalLinks(true);

    setWindowTitle(tr("About"));
}

ActionAbout::~ActionAbout() = default;