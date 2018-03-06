#include <QApplication>

#include "gui/mainwindow.h"
#include "compstate/compstate.h"

int main(int argc, char *argv[]) {
    qRegisterMetaType<cv::Mat>();
    qRegisterMetaType<cv::UMat>();
    qRegisterMetaType<ActionBox *>();
    qRegisterMetaType<std::vector<ActionButton *>>();
    qRegisterMetaType<std::shared_ptr<cv::Rect2d>>();
    qRegisterMetaType<std::shared_ptr<CompetitionState::wall_arr>>();

    QApplication app(argc, argv);

    auto *w = new MainWindow(argc, argv);
    w->show();

    return app.exec();
}
