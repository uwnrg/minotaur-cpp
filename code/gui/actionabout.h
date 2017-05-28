#ifndef ACTIONABOUT_H
#define ACTIONABOUT_H

#include <QDialog>

namespace Ui {
class ActionAbout;
}

class ActionAbout : public QDialog
{
    Q_OBJECT

public:
    explicit ActionAbout(QWidget *parent = 0);
    ~ActionAbout();

private:
    Ui::ActionAbout *ui;
};

#endif // ACTIONABOUT_H
