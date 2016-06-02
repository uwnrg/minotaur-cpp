#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextBrowser>

#include "../controller/controller.h"
#include "actuatorsetup.h"

#define DEFAULT_TITLE "minotaur"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, const char* title = DEFAULT_TITLE);
    QTextEdit* GetLogView();
    ~MainWindow();

public slots:
    void OpenActuatorSetup();

private slots:
    void on_move_button_clicked();

private:
    Ui::MainWindow *ui;
    ActuatorSetup *actuator_setup_window;
    Actuator *m_controller;
};

#endif // MAINWINDOW_H
