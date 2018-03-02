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
 * Drawing 800 buttons is very very slow
 * Allow user to select or deselect an area
 * Add a button to clear selections
 * Dynamic implementation of columns/rows/button array
 * Overlay m_scene over image viewer
*/

//Constructor
GridDisplay::GridDisplay(QWidget *parent) :
    QWidget(parent) {
    m_scene = new QGraphicsScene(this);

//    m_button1 = new QPushButton();
//    m_button1->setText("Button 1");
//    m_button1->setFixedSize(gridSize*5, gridSize);
//    m_button1->setGeometry(QRect(0, gridSize + 5, gridSize*5, gridSize));
    drawButtons();

    //Set up graphics scene and view
    m_scene->setSceneRect(QRect(0, 0, sceneWidth, sceneHeight));   //TODO: set bounding rectangle to m_img.size()
//    m_scene->addWidget(m_button1);

    m_view = new QGraphicsView(m_scene, this);
    drawGrid();

    //TODO: Give QGraphicsScene a grid layout
//    m_layout = new QGraphicsGridLayout;
//    m_scene = new QGraphicsLayoutItem;
//    m_layout->addItem(m_scene, 0, 0, 1, 1);

    //Signals
//    connect(m_button1, SIGNAL(clicked()), this, SLOT(buttonClicked()));
//    connect(m_button1, &QPushButton::clicked, [=](){this->buttonClicked(m_button1);} );
    showView();
}

//Slot
void GridDisplay::buttonClicked(QPushButton *button, int x, int y){
  //  QPushButton *button = (QPushButton *)sender();
    if (squareSelected[x][y]){
        squareSelected[x][y] = false;
        m_button[x][y]->setStyleSheet(buttonStyle);
    } else {
        squareSelected[x][y] = true;
        m_button[x][y]->setStyleSheet(buttonSelectedStyle);
    }
    qDebug() << "Button (" << x << "," << y << ") = " << squareSelected[x][y];
    //Instead of setting text, change button style to filled
}

void GridDisplay::drawButtons(){
    int y = 0;
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
            //  connect(m_button[i], SIGNAL(clicked()), this, SLOT(buttonClicked(m_button[i])));
       }
    }
}

void GridDisplay::showView(){
    m_view->show();
}

//Slot
void GridDisplay::clearSelection(){
    qDebug() << "Clear Selection";
    for (int y = 0; y < rowCount; y++) {
        for (int x = 0; x < columnCount; x++) {
            m_button[x][y]->setStyleSheet(buttonStyle);
            squareSelected[x][y] = false;
        }
    }
}

void GridDisplay::updateScene(){
    m_scene->update();
    showView();
}

void GridDisplay::drawGrid(){
    QPainter painter;
    painter.setPen(Qt::gray);
    painter.setBrush(Qt::green);
    for (int y = 0; y < rowCount; y++) {
        for (int x = 0; x < columnCount; x++) {
            m_scene->addLine(QLine(x*gridSize, y*gridSize, x*gridSize, y*gridSize));
            //painter.drawLine(QLine(x*gridSize, y*gridSize, x*gridSize, y*gridSize));
        }
    }
}