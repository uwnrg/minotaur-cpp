#include <QDebug>
#include "griddisplay.h"

/* TODO (improvements):
 * Qt event handling is slow when handling 800 button slots
 * Change boolean to integer array, add drop down menu to select and set priorities
 * Allow user to select or deselect an area (using mouse events)
 * Dynamic implementation of columns/rows/button array
 * Set bounding rectangle of m_scene to m_img.size()
*/

GridDisplay::GridDisplay(QWidget *parent) :
    QWidget(parent), squareSelected{40, 20} {
    //Set up graphics scene and view
    m_scene = std::make_unique<QGraphicsScene>(this);
    //m_scene = new std::unique_ptr<QGraphicsScene>(this);
    //m_scene->setSceneRect(QRect(0, 0, sceneWidth, sceneHeight));   //TODO: set bounding rectangle to m_img.size()
    m_view = std::make_unique<QGraphicsView>(m_scene.get(), parent);
    //m_view = new std::unique_ptr<QGraphicsView>(m_scene, parent);
    m_view->setStyleSheet("background: transparent");
    m_view->setMinimumSize(sceneWidth, sceneHeight);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

//    drawButtons();
//    drawGrid();
//    showView();
}

void GridDisplay::buttonClicked(int x, int y) {
    if (squareSelected[x][y] > notSelectedWeight){
        squareSelected[x][y] = notSelectedWeight;
        m_button[x][y]->setStyleSheet(buttonStyle);
    } else {
        squareSelected[x][y] = defaultWeight;
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
            squareSelected[x][y] = notSelectedWeight;
            //multiple signals solution 1: signal mapper
//        m_signalmapper->setMapping(m_button[i], i);
//        connect(m_button[i], SIGNAL(clicked()), m_signalmapper, SLOT(map()));
            //multiple signals solution 2: sender() function
            //connect(m_button[i], SIGNAL(clicked()), this, SLOT(buttonClicked()));

            //multiple signals solution 3: lambda functions
            connect(m_button[x][y], &QPushButton::clicked, [=]() { this->buttonClicked(x, y); });
            m_scene->addWidget(m_button[x][y]);
        }
    }
    m_view->adjustSize();
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
            squareSelected[x][y] = notSelectedWeight;
        }
    }
}

void GridDisplay::updateScene() {
    m_scene->update();
    showView();
}

void GridDisplay::showGrid() {
    if (gridDisplayed == false) {
        drawGrid();
        drawButtons();
        showView();
    }
    gridDisplayed = true;
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
    //gridDisplayed = false;
}