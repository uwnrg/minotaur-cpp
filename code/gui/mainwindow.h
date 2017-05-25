#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QTextBrowser>
#include <QKeyEvent>

#include "../controller/controller.h"
#include "../controller/simulator.h"
#include "actuatorsetup.h"
#include "simulatorwindow.h"
#include "../controller/controldelegator.h"

#define DEFAULT_TITLE "minotaur"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, const char* title = DEFAULT_TITLE);
    QTextEdit* getLogView();
    void keyPressEvent(QKeyEvent*);
    ~MainWindow();

public slots:
    void openActuatorSetup();

private slots:
    // Button click events
    void on_move_button_clicked();
    void on_controller_button_clicked();
    // Mouse events
    void mousePressEvent(QMouseEvent *event);

private:
    Ui::MainWindow *ui;
    ActuatorSetup *actuator_setup_window;
    std::shared_ptr<ControlDelegator> m_control_delegator;
	bool eventFilter(QObject*, QEvent*);
};

#endif // MAINWINDOW_H
