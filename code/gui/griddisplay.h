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

class GridDisplay : public QWidget {
Q_OBJECT

public:
    explicit GridDisplay(QWidget *parent);
//    ~GridDisplay();

public Q_SLOTS:
    void clearSelection();
    void showGrid();

protected Q_SLOTS:
    void buttonClicked(int x, int y);
    void hideGrid();

private:
    void showView();
    void updateScene();
    void drawGrid();
    void drawButtons();

    std::unique_ptr<QGraphicsScene> m_scene;
    std::unique_ptr<QGraphicsView> m_view;

    QPushButton *m_button[40][20];
    //std::unique_ptr<QPushButton> m_button[40][20];  //TODO: Replace hardcoded values
    //array2d(QPushButton, 800);
    //bool squareSelected[40][20];
    array2d<int> squareSelected {40, 20};
    //QSignalMapper *m_signalmapper;
    QString buttonStyle = (
        "background-color: rgba(0, 0, 0, 0%);"
        "width: 8px;"
        "height: 8px;"
    );

    QString buttonSelectedStyle = (
        "background-color: rgba(0, 255, 0, 20%);"
        "width: 8px;"
        "height: 8px;"
    );

    const int gridSize = 20;
    const int sceneWidth = 100;     //Default: 800
    const int sceneHeight = 100;    //Default: 400
    int columnCount = sceneWidth / gridSize; //40
    int rowCount = sceneHeight / gridSize;  //20
    bool gridDisplayed = false;

    int notSelectedWeight = -1;
    int defaultWeight = 0;

    //make start and end inputs using Coordinate struct

};

#endif //MINOTAUR_CPP_GRIDDISPLAY_H
