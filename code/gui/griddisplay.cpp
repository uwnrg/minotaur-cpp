#include <QDebug>
#include "griddisplay.h"
/*Have multiple signals (one for each rectangle),
 * when a signal is emitted, it contains the coordinates of the rectangles
 * Only one slot that receives multiple signals
 * Use a signal mapper
 * https://doc.qt.io/archives/qq/qq10-signalmapper.html#thesenderapproach
*/

//Constructor
GridDisplay::GridDisplay(QWidget *parent) :
    QWidget(parent) {

    //Set up buttons
//   QString stylesheet = QString("color: rgba(46, 204, 113, 0.4)");
    m_button1 = new QPushButton();
    m_button1->setText("Button 1");
    m_button1->setFixedSize(gridSize*5, gridSize);
 //   m_button1->setStyle();
 //   m_button1->setStyleSheet(QString("{color:green;}"));    //could not parse stylesheet

    m_button2 = new QPushButton();
    m_button2->setText("Button 2");
    m_button2->setFixedSize(gridSize*5, gridSize);
    m_button2->setGeometry(QRect(0, gridSize + 5, gridSize*5, gridSize));
    drawButtons();

    //Experimental button group setup
//    m_btngroup = new QButtonGroup;
//    m_btngroup->addButton(m_button1, 1); //(button, id)
//    m_btngroup->setExclusive(false); //Multiple buttons can be selected at once

    //Set up graphics scene and view
    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(QRect(0, 0, sceneWidth, sceneHeight));   //TODO: set bounding rectangle to m_img.size()
    m_scene->addWidget(m_button1);
    m_scene->addWidget(m_button2);

    m_view = new QGraphicsView(m_scene, this);
//    drawRectGrid();

    //TODO: Give QGraphicsScene a grid layout
//    m_layout = new QGraphicsGridLayout;
//    m_scene = new QGraphicsLayoutItem;
//    m_layout->addItem(m_scene, 0, 0, 1, 1);

    //Signals
    connect(m_button1, SIGNAL(clicked()), this, SLOT(buttonClicked()));
    connect(m_button2, SIGNAL(clicked()), this, SLOT(buttonClicked()));

    showView(); //Displays everything in m_scene in the camera window
}

void GridDisplay::displayText(){
    m_scene->addText("Hello World");
}

//Slot
void GridDisplay::buttonClicked(){
  //  QPushButton *button = (QPushButton *)sender();
 //   emit digitClicked(button->text()[0].digitValue());
 //   button->setText("Clicked");
    qDebug() << "Button Clicked";
    //Instead of setting text, change button style to filled

}

void GridDisplay::drawRectGrid(){
//    int x = 0, y = 0;
    int columnCount = sceneWidth / gridSize;
    int rowCount = sceneHeight / gridSize;
    int gridCount = columnCount*rowCount;

//    QPainter painter(this);
//    painter.setPen(Qt::green);
    //QPen pen(Qt::green, 3, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);

    //Draw QRects
    QVarLengthArray<QRect, 800> rectangles;
    for (int y = 0; y < rowCount; y++) {
        for (int x = 0; x < columnCount; x++) {
            rectangles.append(QRect(x*gridSize, y*gridSize, gridSize, gridSize));
            m_scene->addRect(QRect(x*gridSize, y*gridSize, gridSize, gridSize));
            //connect(rectangles[x*y], SIGNAL(buttonClicked(int)), this, SLOT(buttonGroupClicked(int)));
        }
    }
}

void GridDisplay::drawButtons(){
    m_signalmapper = new QSignalMapper(this);
    connect(m_signalmapper, SIGNAL(mapped(int)), this, SIGNAL(digitClicked(int)));
    for(int i = 0 ; i < 5 ; i++) {
        QString text = QString::number(i);
        buttons[i] = new QPushButton(text, this);
        buttons[i]->setGeometry(QRect(i*gridSize, 0, gridSize, gridSize));
        buttons[i]->setText("Button");

        //multiple signals solution 1: signal mapper
        m_signalmapper->setMapping(buttons[i], i);
        connect(buttons[i], SIGNAL(clicked()), m_signalmapper, SLOT(map()));

        //multiple signals solution 2: sender() function
        m_scene->addWidget(buttons[i]);   //TODO: Crashes program
        //connect(buttons[i], SIGNAL(clicked()), this, SLOT(buttonClicked()));
    }
}

void GridDisplay::showView(){
    m_view->show();
}

void GridDisplay::updateScene(){
    m_scene->update();
    showView();
}
//void GridDisplay::drawGrid(){
//    QPainter painter;
//    //Draws a filled rectangle on the image viewer
//
//    painter.setPen(Qt::white);
//    painter.setBrush(Qt::green);
//    painter.drawRect(10, 50, gridSize, gridSize);

//    QRect rect(50, 50, gridSize, gridSize);
//
//    m_scene = new QGraphicsScene(this);
//    m_scene->setSceneRect(QRect(0, 0, 1000, 1000));
//    m_scene->addRect(rect);
//}