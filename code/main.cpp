#include "gui/mainwindow.h"
#include "utility/logger.h"
#include "controller/actuator.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow *w = new MainWindow();


    w->show();

    return app.exec();
}
