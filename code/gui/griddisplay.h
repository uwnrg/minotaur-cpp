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

class GridDisplay : public QWidget {
Q_OBJECT

public:
    explicit GridDisplay(QWidget *parent);
//    ~GridDisplay();

public Q_SLOTS:
    void clearSelection();

protected Q_SLOTS:
    void buttonClicked(QPushButton *, int x, int y);
    void drawButtons();
    void hideGrid();

private:
    void showView();
    void updateScene();
    void drawGrid();

    QGraphicsScene *m_scene;
    QGraphicsView *m_view;
    QPushButton *m_button[40][20];  //TODO: Replace hardcoded values
    bool squareSelected[40][20];

    QString buttonStyle = QString(
        "background-color: rgba(0, 0, 0, 0%);"
        "width: 8px;"
        "height: 8px;"
    );
    QString buttonSelectedStyle = QString(
            "background-color: rgba(0, 255, 0, 20%);"
        "width: 8px;"
        "height: 8px;"
    );

    //QSignalMapper *m_signalmapper;

    const int gridSize = 20;
    int sceneWidth = 800;
    int sceneHeight = 400;
    int columnCount = sceneWidth / gridSize; //40
    int rowCount = sceneHeight / gridSize;  //20
    int gridCount = columnCount*rowCount;   //800
};

#endif //MINOTAUR_CPP_GRIDDISPLAY_H
