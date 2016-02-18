#include "gui/mainwindow.h"
#include "utility/logger.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow *w = new MainWindow();

    Logger *logger = new Logger();
    w->show();

    //initialize all the controllers
    std::string message = "Controllers initialized";
    //bool shit = logger.Log(message);

    //delete logger;
    return app.exec();
}
