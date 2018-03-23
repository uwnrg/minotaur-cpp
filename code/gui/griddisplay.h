#ifndef MINOTAUR_CPP_GRIDDISPLAY_H
#define MINOTAUR_CPP_GRIDDISPLAY_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <QDialog>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QGraphicsGridLayout>
#include <QSignalMapper>

#include "../utility/array2d.h"
#include "../utility/utility.h"
#include "../code/camera/imageviewer.h"
#include "../code/utility/array2d.h"
#include "../code/utility/utility.h"

class CameraDisplay;

class GridDisplay : public QWidget {
Q_OBJECT

public:
    explicit GridDisplay(QWidget *parent);

    GridDisplay(ImageViewer *image_viewer, CameraDisplay *camera_display);

    ~GridDisplay() override;

public Q_SLOTS:
    void clearSelection();

    void showGrid();

    void hideGrid();

    void selectRobotPosition(QString);

protected Q_SLOTS:
    void buttonClicked(int x, int y);

private:
    void showView();

    void updateScene();

    void drawGrid();

    void drawButtons();

    void initStartEndPos();

    std::unique_ptr<QGraphicsScene> m_scene;
    std::unique_ptr<QGraphicsView> m_view;

    CameraDisplay *m_camera_display;

    QPushButton *m_button[40][20];
    array2d<int> squareSelected {40, 20};

    const int gridSize = 20;
    const int sceneWidth = 100;     //Default: 800
    const int sceneHeight = 200;    //Default: 400
    int columnCount = sceneWidth / gridSize; //40
    int rowCount = sceneHeight / gridSize;  //20
    bool gridDisplayed = false;

    const int notSelectedWeight = -1;
    const int defaultWeight = 0;
    const int startWeight = -2;
    const int endWeight = -3;

    bool startPosSelected;
    bool endPosSelected;

    struct Coord {
        int x;
        int y;
    };

    std::unique_ptr<Coord> m_start_position;
    std::unique_ptr<Coord> m_end_position;
};

#endif //MINOTAUR_CPP_GRIDDISPLAY_H
