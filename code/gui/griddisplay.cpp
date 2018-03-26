#include <QDebug>
#include "griddisplay.h"
#include "../camera/cameradisplay.h"

/* TODO (improvements):
 * Allow user to select or deselect an area (using mouse events)
 * Dynamic implementation of columns/rows/button array
 * Set bounding rectangle of m_scene to m_img.size()
*/

const QString buttonStyle =
    //clear button style
    "background-color: rgba(0, 0, 0, 0%);"
    "width: 8px;"
    "height: 8px;";

const QString buttonSelectedStyle =
    //green buttons with varying shades
    //%1 is a placeholder for an argument when stylesheet is called
    "background-color: rgba(0, %1, 0, 20%);"
    "width: 8px;"
    "height: 8px;";

const QString startSelectedStyle =
    //red button for robot start configuration
    "background-color: rgba(255, 0, 0, 20%);"
    "width: 8px;"
    "height: 8px;";

const QString endSelectedStyle =
    //blue button for end goal
    "background-color: rgba(0, 0, 255, 20%);"
    "width: 8px;"
    "height: 8px;";

GridDisplay::GridDisplay(ImageViewer *image_viewer, CameraDisplay *camera_display) :
    QWidget(image_viewer),
    m_square_selected(40, 20),
    m_camera_display(camera_display)
//    m_start_position(Coord),
//    m_end_position(Coord)
    {

    //Set up graphics scene and view
    m_scene = std::make_unique<QGraphicsScene>(this);
    m_scene->setSceneRect(QRect(0, 0, m_scene_width, m_scene_height));   //TODO: set bounding rectangle to m_img.size()
    m_view = std::make_unique<QGraphicsView>(m_scene.get(), image_viewer);
    m_view->setStyleSheet("background: transparent");
    m_view->setMinimumSize(m_scene_width, m_scene_height);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void GridDisplay::button_clicked(int x, int y) {
    if (m_start_pos_selected) {
        m_square_selected[x][y] = m_start_weight;
        m_button[x][y]->setStyleSheet(startSelectedStyle);
        m_start_position.x = x;
        m_start_position.y = y;
        m_start_pos_selected = false;
#ifndef NDEBUG
        qDebug() << "Robot Start Position (" << x << "," << y << ") = " << m_square_selected[x][y];
#endif
    } else if (m_end_pos_selected) {
        // Executes if another end position was previously selected
        if (m_end_position.x != m_not_selected_weight
                && m_end_position.y != m_not_selected_weight) {
            m_square_selected[m_end_position.x][m_end_position.y] = -1;
            m_button[m_end_position.x][m_end_position.y]->setStyleSheet(buttonStyle);
        }
        m_square_selected[x][y] = m_end_weight;
        m_button[x][y]->setStyleSheet(endSelectedStyle);
        m_end_position.x = x;
        m_end_position.y = y;
        m_end_pos_selected = false;
#ifndef NDEBUG
        qDebug() << "Robot End Position (" << x << "," << y << ") = " << m_square_selected[x][y];
#endif
    } else if (m_square_selected[x][y] > m_not_selected_weight
           || m_square_selected[x][y] == m_start_weight
           || m_square_selected[x][y] == m_end_weight) {
        m_square_selected[x][y] = m_not_selected_weight;
        m_button[x][y]->setStyleSheet(buttonStyle);
    } else {
        m_square_selected[x][y] = m_camera_display->get_weighting();    //get_weighting() isn't working
        // Sets button to different shades of green based on weighting assigned
        m_button[x][y]->setStyleSheet(buttonSelectedStyle.arg(255 - 10 * m_camera_display->get_weighting()));
    }
#ifndef NDEBUG
    qDebug() << "Button (" << x << "," << y << ") = " << m_square_selected[x][y];
#endif
}

void GridDisplay::draw_buttons() {
    for (int y = 0; y < m_row_count; y++) {
        for (int x = 0; x < m_column_count; x++) {
            QString text = QString::number(x);
            m_button[x][y] = new QPushButton();
            m_button[x][y]->setGeometry(QRect(x * m_grid_size, y * m_grid_size, m_grid_size, m_grid_size));
            m_button[x][y]->setStyleSheet(buttonStyle);
            m_square_selected[x][y] = m_not_selected_weight;
            // Lambda function to receive signals from multiple buttons
            connect(m_button[x][y], &QPushButton::clicked, [=]() { this->button_clicked(x, y); });
            m_scene->addWidget(m_button[x][y]);
        }
    }
    m_view->adjustSize();
}

void GridDisplay::show_view() {
    m_view->show();
}

void GridDisplay::clear_selection() {
#ifndef NDEBUG
    qDebug() << "Clear Selection";
#endif
    for (int y = 0; y < m_row_count; y++) {
        for (int x = 0; x < m_column_count; x++) {
            m_button[x][y]->setStyleSheet(buttonStyle);
            m_square_selected[x][y] = m_not_selected_weight;
        }
    }
}

void GridDisplay::update_scene() {
    m_scene->update();
    show_view();
}

void GridDisplay::show_grid() {
    if (!m_grid_displayed) {
        draw_grid();
        draw_buttons();
        show_view();
        init_start_end_pos();
        m_grid_displayed = true;
    }
}

void GridDisplay::draw_grid() {
    for (int y = 0; y < m_row_count; y++) {
        for (int x = 0; x < m_column_count; x++) {
            m_scene->addLine(QLine(x * m_grid_size, y * m_grid_size, x * m_grid_size, y * m_grid_size));
        }
    }
}

void GridDisplay::hide_grid() {
    //TODO
    //hide or disable qgraphicsview
    //m_grid_displayed = true;
}

void GridDisplay::selectRobotPosition(QString weight_selected) {
    //TODO: Use enum instead of strings
    //TODO: simplify boolean expressions since there's a lot of branching
    if (weight_selected == "Start Configuration") {
        m_start_pos_selected = true;
        m_end_pos_selected = false;
    } else if (weight_selected == "End Configuration") {
        m_start_pos_selected = false;
        m_end_pos_selected = true;
    } else {
        m_start_pos_selected = false;
        m_end_pos_selected = false;
    }
}

void GridDisplay::init_start_end_pos() {
    m_start_position.x = m_not_selected_weight;
    m_start_position.y = m_not_selected_weight;
    m_end_position.x = m_not_selected_weight;
    m_end_position.y = m_not_selected_weight;
}

GridDisplay::~GridDisplay() {
}
