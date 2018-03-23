#include <QDebug>
#include "griddisplay.h"
#include "../camera/cameradisplay.h"

/* TODO (improvements):
 * Allow user to select or deselect an area (using mouse events)
 * Dynamic implementation of columns/rows/button array
 * Set bounding rectangle of m_scene to m_img.size()
*/

const QString GridDisplay::buttonStyle =
    "background-color: rgba(0, 0, 0, 0%);"  //clear
    "width: 8px;"
    "height: 8px;";

const QString GridDisplay::buttonSelectedStyle =
  //  "background-color: rgba(0, 255, 0, 20%);"   //green
    "background-color: rgba(0, %1, 0, 20%);"    //%1 is a placeholder for an argument when stylesheet is called
    "width: 8px;"
    "height: 8px;";

const QString GridDisplay::startSelectedStyle =
    "background-color: rgba(255, 0, 0, 20%);"   //red
    "width: 8px;"
    "height: 8px;";

const QString GridDisplay::endSelectedStyle =
    "background-color: rgba(0, 0, 255, 20%);"   //blue
    "width: 8px;"
    "height: 8px;";

GridDisplay::GridDisplay(ImageViewer *image_viewer, CameraDisplay *camera_display) :
    QWidget(image_viewer),
    squareSelected(40, 20),
    m_camera_display(camera_display),
    m_start_position(std::make_unique<Coord>()),
    m_end_position(std::make_unique<Coord>()) {

    //Set up graphics scene and view
    m_scene = std::make_unique<QGraphicsScene>(this);
    m_scene->setSceneRect(QRect(0, 0, sceneWidth, sceneHeight));   //TODO: set bounding rectangle to m_img.size()
    m_view = std::make_unique<QGraphicsView>(m_scene.get(), image_viewer);
    m_view->setStyleSheet("background: transparent");
}

void GridDisplay::buttonClicked(int x, int y) {
    if (startPosSelected) {
        squareSelected[x][y] = startWeight;
        m_button[x][y]->setStyleSheet(startSelectedStyle);
        m_start_position->x = x;
        m_start_position->y = y;
        startPosSelected = false;
#ifndef NDEBUG
        qDebug() << "Robot Start Position (" << x << "," << y << ") = " << squareSelected[x][y];
#endif
    } else if (endPosSelected) {
        if (m_end_position->x != notSelectedWeight && m_end_position->y != notSelectedWeight) { //If another end position was previously selected
            squareSelected[m_end_position->x][m_end_position->y] = -1;
            m_button[m_end_position->x][m_end_position->y]->setStyleSheet(buttonStyle);
        }
        squareSelected[x][y] = endWeight;
        m_button[x][y]->setStyleSheet(endSelectedStyle);
        m_end_position->x = x;
        m_end_position->y = y;
        endPosSelected = false;
#ifndef NDEBUG
        qDebug() << "Robot End Position (" << x << "," << y << ") = " << squareSelected[x][y];
#endif
    } else if (squareSelected[x][y] > notSelectedWeight
           || squareSelected[x][y] == startWeight
           || squareSelected[x][y] == endWeight) {
        squareSelected[x][y] = notSelectedWeight;
        m_button[x][y]->setStyleSheet(buttonStyle);
    } else {
        squareSelected[x][y] = m_camera_display->getWeighting();
       // m_button[x][y]->setStyleSheet(buttonSelectedStyle);
        //sets button to different shades of green based on weighting assigned
        m_button[x][y]->setStyleSheet(buttonSelectedStyle.arg(255 - 10*m_camera_display->getWeighting()));
    }
#ifndef NDEBUG
    qDebug() << "Button (" << x << "," << y << ") = " << squareSelected[x][y];
#endif
}

void GridDisplay::drawButtons() {
    for (int y = 0; y < rowCount; y++) {
        for (int x = 0; x < columnCount; x++) {
            QString text = QString::number(x);
            m_button[x][y] = new QPushButton();
            m_button[x][y]->setGeometry(QRect(x * gridSize, y * gridSize, gridSize, gridSize));
            m_button[x][y]->setStyleSheet(buttonStyle);
            squareSelected[x][y] = notSelectedWeight;
            //lambda function to receive signals from multiple buttons
            connect(m_button[x][y], &QPushButton::clicked, [=]() { this->buttonClicked(x, y); });
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
            squareSelected[x][y] = notSelectedWeight;
        }
    }
}

void GridDisplay::updateScene() {
    m_scene->update();
    showView();
}

void GridDisplay::showGrid() {
    if (!gridDisplayed) {
        drawGrid();
        drawButtons();
        showView();
        initStartEndPos();
        gridDisplayed = true;
    }
}

void GridDisplay::drawGrid() {
//    QPainter painter;
//    painter.setPen(Qt::gray);
//    painter.setBrush(Qt::green);
    for (int y = 0; y < rowCount; y++) {
        for (int x = 0; x < columnCount; x++) {
            m_scene->addLine(QLine(x*gridSize, y*gridSize, x*gridSize, y*gridSize));
        }
    }
}

void GridDisplay::hideGrid() {
    //TODO
    //hide or disable qgraphicsview
    //gridDisplayed = true;
}

void GridDisplay::selectRobotPosition(QString weightSelected) {
    if (weightSelected == "Start Configuration") {
        startPosSelected = true;
        endPosSelected = false;
    } else if (weightSelected == "End Configuration") {
        startPosSelected = false;
        endPosSelected = true;
    } else {
        startPosSelected = false;
        endPosSelected = false;
    }
}

void GridDisplay::initStartEndPos() {
    m_start_position->x = notSelectedWeight;
    m_start_position->y = notSelectedWeight;
    m_end_position->x = notSelectedWeight;
    m_end_position->y = notSelectedWeight;
}

GridDisplay::~GridDisplay() {

}