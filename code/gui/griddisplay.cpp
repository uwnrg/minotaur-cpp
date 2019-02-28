#include "../camera/cameradisplay.h"
#include "../camera/imageviewer.h"
#include "../utility/logger.h"
#include "griddisplay.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QRubberBand>
#include <QMouseEvent>
#include <QPen>
#include <QBrush>
#include <QGraphicsRectItem>

#ifndef NDEBUG
#include <QDebug>
#endif

/* TODO (improvements):
 * Dynamic implementation of columns/rows/button array
 * Set bounding rectangle of m_scene to m_img.size()
 * Fix seg fault when clicking "Set Grid Path" without any locations selected
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

int GridDisplay::x() {
    return m_view->x();
}

int GridDisplay::y() {
    return m_view->y();
}

GridDisplay::~GridDisplay() = default;

GridDisplay::GridDisplay(ImageViewer *image_viewer, CameraDisplay *camera_display) :
    QWidget(image_viewer),
    m_image_viewer(image_viewer),

    m_column_count(0),
    m_row_count(0),
    m_x(0),
    m_y(0),

    m_button(m_column_count, m_row_count),
    m_square_selected(m_column_count, m_row_count),

    m_camera_display(camera_display) {

    m_scene = std::make_unique<QGraphicsScene>(this);
    m_scene->setSceneRect(QRect(0, 0, SCENE_WIDTH, SCENE_HEIGHT));
    
    //TODO: set bounding rectangle to m_img.size()
    m_default_pen = QPen(Qt::darkGray);
    m_default_pen.setWidth(1);
    m_default_brush = QBrush();
    for (int i=0; i <= 10; i++) {
        m_selected_brushes[i] = QBrush(QColor(0, 255, 0, 255 - 15 * i));
    }
    m_start_brush = QBrush(Qt::red);
    m_end_brush = QBrush(Qt::blue);
    


    m_view = std::make_unique<QGraphicsView>(m_scene.get(), image_viewer);
    m_view->setStyleSheet("background: transparent");
    m_view->setMinimumSize(0, 0);
    
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->hide();
}

void GridDisplay::button_clicked(int x, int y) {
    if (m_start_pos_selected) {
        if (m_start_position.x() != NOT_SELECTED_WEIGHT &&
            m_start_position.y() != NOT_SELECTED_WEIGHT) {
            m_square_selected[m_start_position.x()][m_start_position.y()] = NOT_SELECTED_WEIGHT;
            m_button[m_start_position.x()][m_start_position.y()]->setBrush(m_default_brush);
        }
        m_square_selected[x][y] = START_WEIGHT;
        m_button[x][y]->setBrush(m_start_brush);
        m_start_position = {x, y};
        log() << "Robot Start Position (" << x << "," << y << ") = " << m_square_selected[x][y];
    } else if (m_end_pos_selected) {
        // Executes if another end position was previously selected
        if (m_end_position.x() != NOT_SELECTED_WEIGHT &&
            m_end_position.y() != NOT_SELECTED_WEIGHT) {
            m_square_selected[m_end_position.x()][m_end_position.y()] = NOT_SELECTED_WEIGHT;
            m_button[m_end_position.x()][m_end_position.y()]->setBrush(m_default_brush);
        }
        m_square_selected[x][y] = END_WEIGHT;
        m_button[x][y]->setBrush(m_end_brush);
        m_end_position = {x, y};
        log() << "Robot End Position (" << x << "," << y << ") = " << m_square_selected[x][y];
    } else if (
        m_square_selected[x][y] > NOT_SELECTED_WEIGHT ||
        m_square_selected[x][y] == START_WEIGHT ||
        m_square_selected[x][y] == END_WEIGHT
        ) {
        m_square_selected[x][y] = NOT_SELECTED_WEIGHT;
        m_button[x][y]->setBrush(m_default_brush);
    } else {
        m_square_selected[x][y] = m_camera_display->get_weighting();
        // Sets button to different shades of green based on weighting assigned
        m_button[x][y]->setBrush(m_camera_display->get_weighting() == -1 ? m_default_brush : m_selected_brushes[m_camera_display->get_weighting()]);
    }
#ifndef NDEBUG
    qDebug() << "Button (" << x << "," << y << ") = " << m_square_selected[x][y];
#endif
}

void GridDisplay::draw_buttons() {
    
    for (int y = 0; y < m_row_count; y++) {
        for (int x = 0; x < m_column_count; x++) {
            // QString text = QString::number(x);
            QGraphicsRectItem *rect = m_scene->addRect(QRect(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE), m_default_pen, m_default_brush); 
            m_button[x][y] = rect;
            m_square_selected[x][y] = NOT_SELECTED_WEIGHT;
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
            m_button[x][y]->setBrush(m_default_brush);
            m_square_selected[x][y] = NOT_SELECTED_WEIGHT;
        }
    }
}

void GridDisplay::show_grid() {
    if (!m_grid_displayed) {
        m_row_count = (m_image_viewer->height() / 2) / GRID_SIZE;
        m_column_count = (m_image_viewer->width() / 2) / GRID_SIZE;
        m_scene->setSceneRect(QRect(0, 0, m_column_count * GRID_SIZE + 1, m_row_count * GRID_SIZE + 1));
        m_view->resize(m_column_count * GRID_SIZE, m_row_count * GRID_SIZE);
        move_grid();
        m_button = array2d<QGraphicsRectItem *>(m_column_count, m_row_count);
        m_square_selected = array2d<int>(m_column_count, m_row_count);
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
        m_grid_displayed = false;
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
    m_select_start = QPoint(std::max(std::min(m_mouse_click_start.x(), (int)((m_scene->width() - 1) / GRID_SIZE) - 1), 0),
        std::max(std::min(m_mouse_click_start.y(), (int)((m_scene->height() - 1) / GRID_SIZE) - 1), 0));

    m_rubber_band = std::make_unique<QRubberBand>(QRubberBand::Rectangle, nullptr);
    m_scene->addWidget(m_rubber_band.get());
    m_rubber_band->setGeometry(m_select_start.x(), m_select_start.y(), 2, 2);
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
    bool both_valid = is_valid_coord(top_left) && is_valid_coord(bottom_right);
    if (abs(top_left.x() - bottom_right.x()) < GRID_SIZE &&
        abs(top_left.y() - bottom_right.y()) < GRID_SIZE && 
        both_valid) {
        button_clicked(bottom_right.x() / GRID_SIZE, bottom_right.y() / GRID_SIZE);
        return;
    }
    swap_rect_coords(top_left.x(), top_left.y(), bottom_right.x(), bottom_right.y());

    int x0 = top_left.x() / GRID_SIZE;
    int y0 = top_left.y() / GRID_SIZE;
    int x1 = bottom_right.x() / GRID_SIZE;
    int y1 = bottom_right.y() / GRID_SIZE;
    bool selected = false;
    for (int x = std::max(0, x0); x <= std::min(x1, (int)((m_scene->width() - 1) / GRID_SIZE) - 1); ++x) {
        for (int y = std::max(0, y0); y <= std::min(y1, (int)((m_scene->height() - 1) / GRID_SIZE) - 1); ++y) {
            // If any square in the selection box is not selected,
            // select all the squares in the box
            if (m_square_selected[x][y] < DEFAULT_WEIGHT) {
                rect_select_all_buttons(top_left, bottom_right);
                return;
            } else {
                selected = true;
            }
        }
    }
    // Only deselect buttons if all the buttons in the selection box are selected
    if (selected) {
        rect_deselect_all_buttons(top_left, bottom_right);
    }
}

void GridDisplay::rect_select_all_buttons(
    const vector2i &top_left,
    const vector2i &bottom_right
) {
    int x0 = top_left.x() / GRID_SIZE;
    int y0 = top_left.y() / GRID_SIZE;
    int x1 = bottom_right.x() / GRID_SIZE;
    int y1 = bottom_right.y() / GRID_SIZE;
    for (int x = std::max(0, x0); x <= std::min(x1, (int)((m_scene->width() - 1) / GRID_SIZE) - 1); ++x) {
        for (int y = std::max(0, y0); y <= std::min(y1, (int)((m_scene->height() - 1) / GRID_SIZE) - 1); ++y) {
            m_square_selected[x][y] = m_camera_display->get_weighting();
            m_button[x][y]->setBrush(m_camera_display->get_weighting() == -1 ? m_default_brush : m_selected_brushes[m_camera_display->get_weighting()]);
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
    for (int x = std::max(0, x0); x <= std::min(x1, (int)((m_scene->width() - 1) / GRID_SIZE) - 1); ++x) {
        for (int y = std::max(0, y0); y <= std::min(y1, (int)((m_scene->height() - 1) / GRID_SIZE) - 1); ++y) {
            m_square_selected[x][y] = NOT_SELECTED_WEIGHT;
            m_button[x][y]->setBrush(m_default_brush);
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

bool GridDisplay::is_valid_coord(vector2i &point) {
    if (point.x() < 0 || point.x() > m_scene->width() || point.y() < 0 || point.y() > m_scene->height()) {
        return false;
    }
    return true;
}

void GridDisplay::update_grid_location(double x, double y) {
    m_x = x;
    m_y = y;
    move_grid();
}

void GridDisplay::move_grid() {
    // Update location to match m_x and m_y
    m_view->move((1 + m_x) * (m_image_viewer->width() / 2 - m_scene->width() / 2), (1 - m_y) * (m_image_viewer->height() / 2 - m_scene->height() / 2));
}

QRect GridDisplay::view_geometry() {
    return m_view->geometry();
}

bool GridDisplay::is_displayed() {
    return m_grid_displayed;
}