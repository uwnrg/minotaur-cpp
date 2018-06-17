#include <QApplication>

#include "gui/global.h"
#include "gui/mainwindow.h"
#include "video/modify.h"

Q_DECLARE_METATYPE(cv::Rect2d);
Q_DECLARE_METATYPE(cv::UMat);

int main(int argc, char *argv[]) {
    qRegisterMetaType<cv::UMat>();
    qRegisterMetaType<std::shared_ptr<CompetitionState::wall_arr>>();
    qRegisterMetaType<std::shared_ptr<VideoModifier>>();
    qRegisterMetaType<cv::Rect2d>();

    QApplication app(argc, argv);

    Main::get() = new MainWindow;
    Main::get()->show();

    return app.exec();
}
