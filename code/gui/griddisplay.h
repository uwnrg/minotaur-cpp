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

//#include "camera.h"

class GridDisplay : public QWidget {
Q_OBJECT

public:
    explicit GridDisplay(QWidget *parent = nullptr);
//    ~GridDisplay();

protected Q_SLOTS:
    void buttonClicked();
    void displayText();

Q_SIGNALS:
//    void itemSelected(QGraphicsItem *item);

private:
    void showView();
    void drawRectGrid();
//  void drawGrid();
    void updateScene();
    void drawButtons();

    QGraphicsGridLayout *m_layout;
    QGraphicsScene *m_scene;
    QGraphicsView *m_view;
    QButtonGroup *m_btngroup;
    QPushButton *buttons[5];
    QPushButton *m_button1;
    QPushButton *m_button2;
    QRect *rect;

    QSignalMapper *m_signalmapper;

//    GridDisplay *m_grid_display;

    const int gridSize = 20;
    int sceneWidth = 800;
    int sceneHeight = 400;
};

#endif //MINOTAUR_CPP_GRIDDISPLAY_H
