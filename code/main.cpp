#include "gui/mainwindow.h"
#include "utility/logger.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow *w = new MainWindow();

    Logger::SetStream(w->GetLogView());
    Logger::Log("Logger initialized.");

    w->show();

    return app.exec();
}
