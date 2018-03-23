#include <QApplication>

#include "gui/mainwindow.h"
#include "compstate/compstate.h"
#include "video/modify.h"

Q_DECLARE_METATYPE(cv::Rect2d);

int main(int argc, char *argv[]) {
    qRegisterMetaType<cv::UMat>();
    qRegisterMetaType<std::shared_ptr<CompetitionState::wall_arr>>();
    qRegisterMetaType<std::shared_ptr<VideoModifier>>();
    qRegisterMetaType<cv::Rect2d>();

    QApplication app(argc, argv);

    auto *w = new MainWindow();
    w->show();

    return app.exec();
}
