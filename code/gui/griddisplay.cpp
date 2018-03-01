#include "griddisplay.h"

//Overlays a grid on the camera display
//Constructor
GridDisplay::GridDisplay(QWidget *parent) :
    QWidget(parent) {

    //m_layout = new QVBoxLayout(this);
    //m_grid_display = new GridDisplay(this);

    //setLayout(m_layout);
    //m_layout->addWidget(m_grid_display);
//    ui->setupUI(this);
//    QPixmap pix(":/graphics/img/UWNRG.png");
//    ui->graphicsView->setFrameStyle(QFrame::NoFrame);
//    ui->textBrowser->setFrameStyle(QFrame::NoFrame);
//    ui->textBrowser2->setFrameStyle(QFrame::NoFrame);
//    ui->label_pic->setPixmap(pix);
//    ui->label_pic->setScaledContents(true);
//
//    // make sure github link links to a website
//    ui->textBrowser2->setOpenExternalLinks(true);

//    setWindowTitle(tr("About"));
    createScene();
    drawGrid();
//
////    m_view = new QGraphicsView(m_scene);
//    m_view = new QGraphicsView(m_scene, this);
//    m_view->setScene(m_scene);
//    m_view->show();
}

void GridDisplay::drawGrid(){
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
}

void GridDisplay::createScene(){
//    int x = 0, y = 0;
    int sceneWidth = 800;
    int sceneHeight = 400;
    int columnCount = sceneWidth / gridSize;
    int rowCount = sceneHeight / gridSize;
    int gridCount = columnCount*rowCount;

    QPainter painter(this);
    painter.setPen(Qt::green);
    //QPen pen(Qt::green, 3, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);

    m_button1 = new QPushButton();

    m_buttongroup = new QButtonGroup;
    m_buttongroup->addButton(m_button1, 1);
    m_buttongroup->setExclusive(true);

    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(QRect(0, 0, sceneWidth, sceneHeight));   //TODO: set bounding rectangle to m_img.size()

    m_scene->addText("Hello World");

//    m_scene->addRect(QRect(0, gridSize, gridSize, gridSize));
//    m_scene->addRect(QRect(gridSize, gridSize, gridSize, gridSize));

   // m_scene->addWidget(m_button1);

    //Have multiple signals (one for each rectangle), when a signal is emitted, it contains the coordinates of the rectangles
    //Only one slot that receives multiple signals
    QVarLengthArray<QRect, 800> rectangles;
    for (int y = 0; y < rowCount; y++) {
        for (int x = 0; x < columnCount; x++) {
            rectangles.append(QRect(x*gridSize, y*gridSize, gridSize, gridSize));
            m_scene->addRect(QRect(x*gridSize, y*gridSize, gridSize, gridSize));
            //connect(rectangles[x*y], SIGNAL(buttonClicked(int)), this, SLOT(buttonGroupClicked(int)));
        }
    }

    m_view = new QGraphicsView(m_scene, this);
    m_view->show();
}
