#include <QDebug>
#include "griddisplay.h"
/*Have multiple signals (one for each rectangle),
 * when a signal is emitted, it contains the coordinates of the rectangles
 * Only one slot that receives multiple signals
 * Use a signal mapper
 * https://doc.qt.io/archives/qq/qq10-signalmapper.html#thesenderapproach
 *
 * Output to boolean array (may have to change to int array later)
 *
 * TODO:
 * Optimize button adding (drawing and adding 800 buttons is very very slow)
 * Allow user to select or deselect an area
 * Dynamic implementation of columns/rows/button array
 * Change boolean to integer array, add drop down menu to select and set priorities
 *
*/

//Constructor
GridDisplay::GridDisplay(QWidget *parent) :
    QWidget(parent) {
    //Set up graphics scene and view
    m_scene = new QGraphicsScene(this);
//    m_scene->setParent(parent);
//    m_scene->show();
    m_scene->setSceneRect(QRect(0, 0, sceneWidth, sceneHeight));   //TODO: set bounding rectangle to m_img.size()

    m_view = new QGraphicsView(m_scene, parent);
    m_view->setStyleSheet("background: transparent");

    drawButtons();
    drawGrid();
    showView();
}

void GridDisplay::buttonClicked(QPushButton *button, int x, int y) {
    if (squareSelected[x][y]){
        squareSelected[x][y] = false;
        m_button[x][y]->setStyleSheet(buttonStyle);
    } else {
        squareSelected[x][y] = true;
        m_button[x][y]->setStyleSheet(buttonSelectedStyle);
    }
#ifndef NDEBUG
    qDebug() << "Button (" << x << "," << y << ") = " << squareSelected[x][y];
#endif
}

void GridDisplay::drawButtons() {
//    m_signalmapper = new QSignalMapper(this);
//    connect(m_signalmapper, SIGNAL(mapped(int)), this, SIGNAL(buttonClicked(int)));
    for (int y = 0; y < rowCount; y++) {
        for (int x = 0; x < columnCount; x++) {
            QString text = QString::number(x);
            m_button[x][y] = new QPushButton();
            m_button[x][y]->setGeometry(QRect(x * gridSize, y * gridSize, gridSize, gridSize));
            m_button[x][y]->setStyleSheet(buttonStyle);
            squareSelected[x][y] = false;
            //multiple signals solution 1: signal mapper
//        m_signalmapper->setMapping(m_button[i], i);
//        connect(m_button[i], SIGNAL(clicked()), m_signalmapper, SLOT(map()));
            //multiple signals solution 2: sender() function
            //connect(m_button[i], SIGNAL(clicked()), this, SLOT(buttonClicked()));

            //multiple signals solution 3: lambda functions
            connect(m_button[x][y], &QPushButton::clicked, [=]() { this->buttonClicked(m_button[x][y], x, y); });
            m_scene->addWidget(m_button[x][y]);
       }
    }
}

void GridDisplay::showView() {
    m_view->show();
}

void GridDisplay::clearSelection() {
#ifndef NDEBUG
    qDebug() << "Clear Selection";
#endif
    for (int y = 0; y < rowCount; y++) {
        for (int x = 0; x < columnCount; x++) {
            m_button[x][y]->setStyleSheet(buttonStyle);
            squareSelected[x][y] = false;
        }
    }
}

void GridDisplay::updateScene() {
    m_scene->update();
    showView();
}

void GridDisplay::drawGrid() {
    QPainter painter;
    painter.setPen(Qt::gray);
    painter.setBrush(Qt::green);
    for (int y = 0; y < rowCount; y++) {
        for (int x = 0; x < columnCount; x++) {
            m_scene->addLine(QLine(x*gridSize, y*gridSize, x*gridSize, y*gridSize));
        }
    }
}

void GridDisplay::hideGrid() {
    //TODO
}