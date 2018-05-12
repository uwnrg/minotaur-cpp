#include "griddisplay.h"
#include "gridbutton.h"
#include "../camera/cameradisplay.h"
#include "../utility/logger.h"

#ifndef NDEBUG
#include <QDebug>
#endif

/* TODO (improvements):
 * Allow user to select or deselect an area (using mouse events)
 * Dynamic implementation of columns/rows/button array
 * Set bounding rectangle of m_scene to m_img.size()
*/

const QString buttonStyle =
    // Clear button style
    "background-color: rgba(0, 0, 0, 0%);"
        "width: 8px;"
        "height: 8px;";

const QString buttonSelectedStyle =
    // Green buttons with varying shades
    // %1 is a placeholder for an argument when stylesheet is called
    "background-color: rgba(0, %1, 0, 20%);"
        "width: 8px;"
        "height: 8px;";

const QString startSelectedStyle =
    // Red button for robot start configuration
    "background-color: rgba(255, 0, 0, 20%);"
        "width: 8px;"
        "height: 8px;";

const QString endSelectedStyle =
    // Blue button for end goal
    "background-color: rgba(0, 0, 255, 20%);"
        "width: 8px;"
        "height: 8px;";

static void swap_rect_coords(int &x1, int &y1, int &x2, int &y2) {
    if (x1 > x2 && y1 > y2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    else if (x1 > x2 && y1 < y2) {
        std::swap(x1, x2);
    }
    else if (x1 < x2 && y1 > y2) {
        std::swap(y1, y2);
    }
}

GridDisplay::GridDisplay(ImageViewer *image_viewer, CameraDisplay *camera_display) :
    QWidget(image_viewer),
    m_square_selected(40, 20),
    m_camera_display(camera_display) {
    // Set up graphics scene and view
    m_scene = std::make_unique<QGraphicsScene>(this);
    m_scene->setSceneRect(QRect(0, 0, SCENE_WIDTH, SCENE_HEIGHT));   //TODO: set bounding rectangle to m_img.size()
    m_view = std::make_unique<QGraphicsView>(m_scene.get(), image_viewer);
    m_view->setStyleSheet("background: transparent");
    m_view->setMinimumSize(SCENE_WIDTH, SCENE_HEIGHT);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void GridDisplay::button_clicked(int x, int y) {
    if (m_start_pos_selected) {
        m_square_selected[x][y] = START_WEIGHT;
        m_button[x][y]->setStyleSheet(startSelectedStyle);
        set_coordinates(m_start_position, x, y);
        m_start_pos_selected = false;

    log() << "Robot Start Position (" << x << "," << y << ") = " << m_square_selected[x][y];
    } else if (m_end_pos_selected) {
        // Executes if another end position was previously selected
        if (
            m_end_position.x != NOT_SELECTED_WEIGHT &&
            m_end_position.y != NOT_SELECTED_WEIGHT
        ) {
            m_square_selected[m_end_position.x][m_end_position.y] = -1;
            m_button[m_end_position.x][m_end_position.y]->setStyleSheet(buttonStyle);
        }
        m_square_selected[x][y] = END_WEIGHT;
        m_button[x][y]->setStyleSheet(endSelectedStyle);
        set_coordinates(m_end_position, x, y);
        m_end_pos_selected = false;

    log() << "Robot End Position (" << x << "," << y << ") = " << m_square_selected[x][y];

    } else if (
        m_square_selected[x][y] > NOT_SELECTED_WEIGHT ||
        m_square_selected[x][y] == START_WEIGHT ||
        m_square_selected[x][y] == END_WEIGHT
    ) {
        m_square_selected[x][y] = NOT_SELECTED_WEIGHT;
        m_button[x][y]->setStyleSheet(buttonStyle);
    } else {
        m_square_selected[x][y] = m_camera_display->get_weighting();
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
            m_button[x][y] = new GridButton(this);
            m_button[x][y]->setGeometry(QRect(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE));
            m_button[x][y]->setStyleSheet(buttonStyle);
            m_square_selected[x][y] = NOT_SELECTED_WEIGHT;
            // Lambda function to receive signals from multiple buttons
            //connect(m_button[x][y], &GridButton::clicked, [=]() { this->button_clicked(x, y); });
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
            m_square_selected[x][y] = NOT_SELECTED_WEIGHT;
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
    } else {
        // Show all the elements instead of redrawing them

    }
}

void GridDisplay::draw_grid() {
    for (int y = 0; y < m_row_count; y++) {
        for (int x = 0; x < m_column_count; x++) {
            m_scene->addLine(QLine(x * GRID_SIZE, y * GRID_SIZE, x * GRID_SIZE, y * GRID_SIZE));
        }
    }
}

void GridDisplay::hide_grid() {
    // TODO
    for (int y = 0; y < m_row_count; y++) {
        for (int x = 0; x < m_column_count; x++) {
            //m_button[x][y]->setVisible(false);
            m_button[x][y]->hide();
            //m_scene->;
        }
    }
    qDebug() << "Hide Grid";

}

void GridDisplay::select_robot_position(QString weight_selected) {
    // TODO: Use enum instead of strings
    m_start_pos_selected = (weight_selected == "Start Configuration");
    m_end_pos_selected = (weight_selected == "End Configuration");
}

void GridDisplay::init_start_end_pos() {
    set_coordinates(m_start_position, NOT_SELECTED_WEIGHT, NOT_SELECTED_WEIGHT);
    set_coordinates(m_end_position, NOT_SELECTED_WEIGHT, NOT_SELECTED_WEIGHT);
}

void GridDisplay::mousePressEvent(QMouseEvent *ev) {
    QWidget::mousePressEvent(ev);
    m_select_start = QPoint(m_mouse_click_start.x, m_mouse_click_start.y);

    m_rubber_band = std::make_unique<QRubberBand>(QRubberBand::Rectangle, this);
    //m_rubber_band = new QRubberBand(QRubberBand::Rectangle, this);
    m_rubber_band->setGeometry(m_mouse_click_start.x, m_mouse_click_start.y, 2, 2);
    m_rubber_band->show();
    m_rubber_band->raise();
}

void GridDisplay::mouseReleaseEvent(QMouseEvent *ev) {
    QWidget::mouseReleaseEvent(ev);
    m_rubber_band->hide();
    rect_select_buttons(m_mouse_click_start, m_mouse_click_release);
}

void GridDisplay::mouseMoveEvent(QMouseEvent *ev) {
#ifndef NDEBUG
    qDebug() << "Mouse move event: " << ev->pos() << endl;
#endif
    QWidget::mouseMoveEvent(ev);
    //m_rubber_band->resize(QRect(m_select_start, QPoint(m_mouse_move.x, m_mouse_move.y)).size());
    m_rubber_band->setGeometry(QRect(m_select_start, QPoint(m_mouse_move.x, m_mouse_move.y)));
    m_rubber_band->raise();
}

void GridDisplay::rect_select_buttons(Coord top_left, Coord bottom_right) {
    // If only one button is selected
    if (abs(top_left.x - bottom_right.x) < GRID_SIZE && abs(top_left.y - bottom_right.y) < GRID_SIZE) {
        button_clicked(bottom_right.x / GRID_SIZE, bottom_right.y / GRID_SIZE);
        return;
    }

    swap_rect_coords(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
    for (int x = top_left.x / GRID_SIZE; x <= bottom_right.x / GRID_SIZE; x++) {
        for (int y = top_left.y / GRID_SIZE; y <= bottom_right.y / GRID_SIZE; y++) {
            // If any square in the selection box is not selected, select all the squares in the box
            if (m_square_selected[x][y] < DEFAULT_WEIGHT) {
                rect_select_all_buttons(top_left, bottom_right);
                return;
            }
        }
    }
    // Only deselect buttons if all the buttons in the selection box are selected
    rect_deselect_all_buttons(top_left, bottom_right);
}

void GridDisplay::rect_select_all_buttons(const Coord top_left, const Coord bottom_right) {
    for (int x = top_left.x / GRID_SIZE; x <= bottom_right.x / GRID_SIZE; x++) {
        for (int y = top_left.y / GRID_SIZE; y <= bottom_right.y / GRID_SIZE; y++) {
            m_square_selected[x][y] = m_camera_display->get_weighting();
            m_button[x][y]->setStyleSheet(buttonSelectedStyle.arg(255 - 10 * m_camera_display->get_weighting()));
        }
    }
}

void GridDisplay::rect_deselect_all_buttons(const Coord top_left, const Coord bottom_right) {
    for (int x = top_left.x / GRID_SIZE; x <= bottom_right.x / GRID_SIZE; x++) {
        for (int y = top_left.y / GRID_SIZE; y <= bottom_right.y / GRID_SIZE; y++) {
            m_square_selected[x][y] = NOT_SELECTED_WEIGHT;
            m_button[x][y]->setStyleSheet(buttonStyle);
        }
    }
}

void GridDisplay::set_coordinates(Coord& coord, const int x, const int y) {
    coord.x = x;
    coord.y = y;
}

void GridDisplay::set_coordinates(Coord& coord, const QPoint& pos) {
    set_coordinates(coord, pos.x(), pos.y());
}

int GridDisplay::get_row_count() {
    return m_row_count;
}

int GridDisplay::get_column_count() {
    return m_column_count;
}

void GridDisplay::set_mouse_start(const QPoint& pos) {
    set_coordinates(m_mouse_click_start, pos.x(), pos.y());
}

void GridDisplay::set_mouse_move(const QPoint& pos) {
    set_coordinates(m_mouse_move, pos.x(), pos.y());
}

void GridDisplay::set_mouse_release(const QPoint& pos) {
    set_coordinates(m_mouse_click_release, pos.x(), pos.y());
}

bool GridDisplay::is_valid_coord(const Coord &coord) {
    // Check if coordinate is within the widget

}

GridDisplay::~GridDisplay () {

}
