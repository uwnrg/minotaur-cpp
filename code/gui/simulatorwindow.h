#ifndef SIMULATORWINDOW_H
#define SIMULATORWINDOW_H

#include <QDialog>

namespace Ui {
class SimulatorWindow;
}

class SimulatorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SimulatorWindow(QWidget *parent = 0);
    ~SimulatorWindow();

private:
    Ui::SimulatorWindow *ui;
};

#endif // SIMULATORWINDOW_H
