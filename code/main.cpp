#include "gui/mainwindow.h"
#include "utility/logger.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow *w = new MainWindow();

    w->show();


    std::string message = "Logger initialized";
    Logger::Log(message);

    return app.exec();
}
