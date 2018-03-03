#ifndef ACTIONABOUT_H
#define ACTIONABOUT_H

#include <QDialog>

#include <memory>

namespace Ui {
class ActionAbout;
}

class ActionAbout : public QDialog
{
    Q_OBJECT

public:
    explicit ActionAbout(QWidget *parent = nullptr);
    ~ActionAbout();

private:
    std::unique_ptr<Ui::ActionAbout> ui;
};

#endif // ACTIONABOUT_H
