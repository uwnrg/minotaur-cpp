#include <QApplication>

#include "gui/mainwindow.h"

int main(int argc, char *argv[]) {
    qRegisterMetaType<cv::Mat>();
    qRegisterMetaType<cv::UMat>();
    qRegisterMetaType<ActionBox *>();
    qRegisterMetaType<std::vector<ActionButton *>>();

    QApplication app(argc, argv);

    auto *w = new MainWindow();
    w->show();

    return app.exec();
}
