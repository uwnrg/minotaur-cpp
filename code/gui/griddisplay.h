#ifndef MINOTAUR_CPP_GRIDDISPLAY_H
#define MINOTAUR_CPP_GRIDDISPLAY_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <QDialog>
#include <QVBoxLayout>
#include <QButtonGroup>

//#include "camera.h"

//namespace Ui {
//    class GridDisplay;
//}

class GridDisplay : public QWidget {
    Q_OBJECT

public:
    explicit GridDisplay(QWidget *parent = nullptr);
//    ~GridDisplay();

private:
//    Ui::GridDisplay *ui;

    void drawGrid();
    void createScene();

    QVBoxLayout *m_layout;
    QGraphicsScene *m_scene;
    QGraphicsView *m_view;
    QRect *rect;

    QPushButton *m_button1;
    QButtonGroup *m_buttongroup;

    GridDisplay *m_grid_display;

    const int gridSize = 20;

    Q_SIGNALS:
//    void itemSelected(QGraphicsItem *item);

};


#endif //MINOTAUR_CPP_GRIDDISPLAY_H
