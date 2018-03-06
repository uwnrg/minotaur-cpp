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
    void showGrid();

protected Q_SLOTS:
    void buttonClicked(QPushButton *, int x, int y);
    void hideGrid();

private:
    void showView();
    void updateScene();
    void drawGrid();
    void drawButtons();

    QGraphicsScene *m_scene;
    QGraphicsView *m_view;
    QPushButton *m_button[40][20];  //TODO: Replace hardcoded values
    bool squareSelected[40][20];
    //QSignalMapper *m_signalmapper;


    QString buttonStyle = QString (
        "background-color: rgba(0, 0, 0, 0%);"
        "width: 8px;"
        "height: 8px;"
    );
    QString buttonSelectedStyle = QString (
        "background-color: rgba(0, 255, 0, 20%);"
        "width: 8px;"
        "height: 8px;"
    );

    const int gridSize = 20;
    const int sceneWidth = 800;
    const int sceneHeight = 400;
    int columnCount = sceneWidth / gridSize; //40
    int rowCount = sceneHeight / gridSize;  //20
};

#endif //MINOTAUR_CPP_GRIDDISPLAY_H
