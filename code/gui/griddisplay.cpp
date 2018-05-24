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

enum {
    END_WEIGHT = -3,
    START_WEIGHT = -2,
    NOT_SELECTED_WEIGHT = -1,
    DEFAULT_WEIGHT = 0,     // All selected squares should have values greater or equal to 0
    GRID_SIZE = 10,         // Size in pixels
    SCENE_WIDTH = 400,      // Default: 800
    SCENE_HEIGHT = 300,     // Default: 400
    MAX_BUTTONS_X = 50,
    MAX_BUTTONS_Y = 50
};

const QString buttonStyle =
    // Clear button style
    "background-color: rgba(0, 0, 0, 0%);"
    "width: 0px;"
    "height: 0px;"
    "border-style: none;";

const QString buttonSelectedStyle =
    // Green buttons with varying shades
    // %1 is a placeholder for an argument when stylesheet is called
    "background-color: rgba(0, %1, 0, 20%);"
    "width: 0px;"
    "height: 0px;"
    "border-style: none;";

const QString startSelectedStyle =
    // Red button for robot start configuration
    "background-color: rgba(255, 0, 0, 20%);"
    "width: 0px;"
    "height: 0px;"
    "border-style: none;";

const QString endSelectedStyle =
    // Blue button for end goal
    "background-color: rgba(0, 0, 255, 20%);"
    "width: 0px;"
    "height: 0px;"
    "border-style: none;";

// Swap two coordinates so that x is the top left point and y is the bottom right
static void swap_rect_coords(int &x1, int &y1, int &x2, int &y2) {
    if (x1 > x2 && y1 > y2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    } else if (x1 > x2 && y1 < y2) {
        std::swap(x1, x2);
    } else if (x1 < x2 && y1 > y2) {
        std::swap(y1, y2);
    }
}

int GridDisplay::default_weight() {
    return DEFAULT_WEIGHT;
}

int GridDisplay::grid_size() {
    return GRID_SIZE;
}

double GridDisplay::scene_width() {
    return m_scene->width();
}

double GridDisplay::scene_height() {
    return m_scene->height();
}

double GridDisplay::parent_width() {
    return m_image_viewer->width();
}

double GridDisplay::parent_height() {
    return m_image_viewer->height();
}

double GridDisplay::x() {
    return m_view->x();
}

double GridDisplay::y() {
    return m_view->y();
}

GridDisplay::GridDisplay(ImageViewer *image_viewer, CameraDisplay *camera_display) :
    QWidget(image_viewer),
    m_image_viewer(image_viewer),

    m_column_count(0),
    m_row_count(0),

    m_button(m_column_count, m_row_count),
    m_square_selected(m_column_count, m_row_count),

    m_camera_display(camera_display) {

    m_scene = std::make_unique<QGraphicsScene>(this);
    m_scene->setSceneRect(QRect(0, 0, SCENE_WIDTH, SCENE_HEIGHT));
    
    //TODO: set bounding rectangle to m_img.size()

    m_view = std::make_unique<QGraphicsView>(m_scene.get(), image_viewer);
    m_view->setStyleSheet("background: transparent");
    m_view->setMinimumSize(0, 0);
    
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->hide();
}

void GridDisplay::button_clicked(int x, int y) {
    if (m_start_pos_selected) {
        m_square_selected[x][y] = START_WEIGHT;
        m_button[x][y]->setStyleSheet(startSelectedStyle);
        m_start_position = {x, y};
        m_start_pos_selected = false;
        log() << "Robot Start Position (" << x << "," << y << ") = " << m_square_selected[x][y];
    } else if (m_end_pos_selected) {
        // Executes if another end position was previously selected
        if (m_end_position.x() != NOT_SELECTED_WEIGHT &&
            m_end_position.y() != NOT_SELECTED_WEIGHT) {
            m_square_selected[m_end_position.x()][m_end_position.y()] = -1;
            m_button[m_end_position.x()][m_end_position.y()]->setStyleSheet(buttonStyle);
        }
        m_square_selected[x][y] = END_WEIGHT;
        m_button[x][y]->setStyleSheet(endSelectedStyle);
        m_end_position = {x, y};
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

void GridDisplay::show_grid() {
    m_row_count = (m_image_viewer->height() / 2) / GRID_SIZE;
    m_column_count = (m_image_viewer->width() / 2) / GRID_SIZE;
    m_scene->setSceneRect(QRect(0, 0, m_column_count * GRID_SIZE, m_row_count * GRID_SIZE));
    m_view->resize(m_column_count * GRID_SIZE, m_row_count * GRID_SIZE);
    m_view->move(m_image_viewer->width() / 2 - m_scene->width() / 2, m_image_viewer->height() / 2 - m_scene->height() / 2);
    m_button = array2d<GridButton *>(m_column_count, m_row_count);
    m_square_selected = array2d<int>(m_column_count, m_row_count);
    if (!m_grid_displayed) {
        draw_grid();
        draw_buttons();
        show_view();
        init_start_end_pos();
        m_grid_displayed = true;
    } else {
        show_view();
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
    if (m_grid_displayed) {
        m_view->hide();
#ifdef NDEBUG
        qDebug() << "Hide grid";
#endif
    }
}

void GridDisplay::select_robot_position(QString weight_selected) {
    // TODO: Use enum instead of strings
    m_start_pos_selected = (weight_selected == "Start Configuration");
    m_end_pos_selected = (weight_selected == "End Configuration");
}

void GridDisplay::init_start_end_pos() {
    m_start_position = {NOT_SELECTED_WEIGHT, NOT_SELECTED_WEIGHT};
    m_end_position = {NOT_SELECTED_WEIGHT, NOT_SELECTED_WEIGHT};
}

void GridDisplay::mousePressEvent(QMouseEvent *ev) {
    QWidget::mousePressEvent(ev);
    m_select_start = QPoint(m_mouse_click_start.x(), m_mouse_click_start.y());

    m_rubber_band = std::make_unique<QRubberBand>(QRubberBand::Rectangle, nullptr);
    m_scene->addWidget(m_rubber_band.get());
    m_rubber_band->setGeometry(m_mouse_click_start.x(), m_mouse_click_start.y(), 2, 2);
    m_rubber_band->show();
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
    m_rubber_band->setGeometry(QRect(
        QPoint(m_mouse_click_start.x(), m_mouse_click_start.y()),
        QPoint(m_mouse_move.x(), m_mouse_move.y())).normalized());
}

void GridDisplay::rect_select_buttons(
    vector2i top_left,
    vector2i bottom_right
) {
    // If only one button is selected
    is_valid_coord(top_left);
    is_valid_coord(bottom_right);
    if (abs(top_left.x() - bottom_right.x()) < GRID_SIZE &&
        abs(top_left.y() - bottom_right.y()) < GRID_SIZE) {
        button_clicked(bottom_right.x() / GRID_SIZE, bottom_right.y() / GRID_SIZE);
        return;
    }
    swap_rect_coords(top_left.x(), top_left.y(), bottom_right.x(), bottom_right.y());
    int x0 = top_left.x() / GRID_SIZE;
    int y0 = top_left.y() / GRID_SIZE;
    int x1 = bottom_right.x() / GRID_SIZE;
    int y1 = bottom_right.y() / GRID_SIZE;
    for (int x = x0; x <= x1; ++x) {
        for (int y = y0; y <= y1; ++y) {
            // If any square in the selection box is not selected,
            // select all the squares in the box
            if (m_square_selected[x][y] < DEFAULT_WEIGHT) {
                rect_select_all_buttons(top_left, bottom_right);
                return;
            }
        }
    }
    // Only deselect buttons if all the buttons in the selection box are selected
    rect_deselect_all_buttons(top_left, bottom_right);
}

void GridDisplay::rect_select_all_buttons(
    const vector2i &top_left,
    const vector2i &bottom_right
) {
    int x0 = top_left.x() / GRID_SIZE;
    int y0 = top_left.y() / GRID_SIZE;
    int x1 = bottom_right.x() / GRID_SIZE;
    int y1 = bottom_right.y() / GRID_SIZE;
    for (int x = x0; x <= x1; ++x) {
        for (int y = y0; y <= y1; ++y) {
            m_square_selected[x][y] = m_camera_display->get_weighting();
            m_button[x][y]->setStyleSheet(buttonSelectedStyle.arg(255 - 10 * m_camera_display->get_weighting()));
        }
    }
}

void GridDisplay::rect_deselect_all_buttons(
    const vector2i &top_left,
    const vector2i &bottom_right
) {
    int x0 = top_left.x() / GRID_SIZE;
    int y0 = top_left.y() / GRID_SIZE;
    int x1 = bottom_right.x() / GRID_SIZE;
    int y1 = bottom_right.y() / GRID_SIZE;
    for (int x = x0; x <= x1; ++x) {
        for (int y = y0; y <= y1; ++y) {
            m_square_selected[x][y] = NOT_SELECTED_WEIGHT;
            m_button[x][y]->setStyleSheet(buttonStyle);
        }
    }
}

int GridDisplay::get_num_rows() const {
    return m_row_count;
}

int GridDisplay::get_num_cols() const {
    return m_column_count;
}

const vector2i &GridDisplay::get_pos_start() const {
    return m_start_position;
}

const vector2i &GridDisplay::get_pos_end() const {
    return m_end_position;
}

array2d<int> &GridDisplay::selected() {
    return m_square_selected;
}

void GridDisplay::set_mouse_start(const QPoint &pos) {
    m_mouse_click_start = pos;
}

void GridDisplay::set_mouse_move(const QPoint &pos) {
    m_mouse_move = pos;
}

void GridDisplay::set_mouse_release(const QPoint &pos) {
    m_mouse_click_release = pos;
}

vector2i GridDisplay::get_mouse_start() {
    return m_mouse_click_start;
}

void GridDisplay::is_valid_coord(vector2i &point) {
    point.x() = (point.x() < 0) ? 0 : point.x();
    point.x() = (point.x() > m_scene->width()) ? m_scene->width() - 1 : point.x();
    point.y() = (point.y() < 0) ? 0 : point.y();
    point.y() = (point.y() > m_scene->height()) ? m_scene->height() - 1 : point.y();
}
