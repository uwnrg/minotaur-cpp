#include <QApplication>

#include "gui/mainwindow.h"

int main(int argc, char *argv[]) {
    qRegisterMetaType<cv::Mat>();
    QApplication app(argc, argv);

    auto *w = new MainWindow(argc, argv);
    w->show();

    return app.exec();
}
