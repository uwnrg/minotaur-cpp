#ifndef MINOTAUR_CPP_GRIDDISPLAY_H
#define MINOTAUR_CPP_GRIDDISPLAY_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <QDialog>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QGraphicsGridLayout>
#include <QSignalMapper>
#include <QMouseEvent>
#include <QRubberBand>

#include "../utility/array2d.h"
#include "../utility/utility.h"
#include "../utility/vector.h"
#include "../camera/cameradisplay.h"
#include "../camera/imageviewer.h"

class CameraDisplay;
class GridButton;

class GridDisplay : public QWidget {
Q_OBJECT

public:
    enum {
        END_WEIGHT = -3,
        START_WEIGHT = -2,
        NOT_SELECTED_WEIGHT = -1,
        DEFAULT_WEIGHT = 0,     // All selected squares should have values greater or equal to 0
        GRID_SIZE = 20,
        SCENE_WIDTH = 100,      // Default: 800
        SCENE_HEIGHT = 100,     // Default: 400
        MAX_BUTTONS_X = 50,
        MAX_BUTTONS_Y = 50
    };

    GridDisplay(ImageViewer *image_viewer, CameraDisplay *camera_display);

    void mousePressEvent(QMouseEvent *ev) override;

    void mouseReleaseEvent(QMouseEvent *ev) override;

    void mouseMoveEvent(QMouseEvent *ev) override;

    void set_vector2iinates(vector2i &vector2i, const int x, const int y);

    void set_vector2iinates(vector2i &vector2i, const QPoint &pos);

    void set_mouse_start(const QPoint &pos);

    void set_mouse_move(const QPoint &pos);

    void set_mouse_release(const QPoint &pos);

    vector2i get_mouse_start ();

    int get_num_rows() const;
    int get_num_cols() const;

    const vector2i &get_pos_start() const;
    const vector2i &get_pos_end() const;

public Q_SLOTS:

    void clear_selection();

    void show_grid();

    void hide_grid();

    void select_robot_position(QString);

protected Q_SLOTS:

    void button_clicked(int x, int y);

private:
    void show_view();

    void update_scene();

    void draw_grid();

    void draw_buttons();

    void init_start_end_pos();

    void rect_select_buttons(
        vector2i top_left,
        vector2i bottom_right
    );

    void rect_select_all_buttons(
        const vector2i &top_left,
        const vector2i &bottom_right
    );

    void rect_deselect_all_buttons(
        const vector2i &top_left,
        const vector2i &bottom_right
    );

    void is_valid_coord(vector2i& point);

    std::unique_ptr<QGraphicsScene> m_scene;
    std::unique_ptr<QGraphicsView> m_view;

    CameraDisplay *m_camera_display;

    GridButton *m_button[MAX_BUTTONS_X][MAX_BUTTONS_Y];
    array2d<int> m_square_selected{MAX_BUTTONS_X, MAX_BUTTONS_Y};

    int m_column_count = SCENE_WIDTH / GRID_SIZE;   // Default: 40
    int m_row_count = SCENE_HEIGHT / GRID_SIZE;     // Default: 20
    bool m_grid_displayed = false;

    bool m_start_pos_selected = false;
    bool m_end_pos_selected = false;

    vector2i m_start_position;
    vector2i m_end_position;

    vector2i m_mouse_click_start;
    vector2i m_mouse_move;
    vector2i m_mouse_click_release;

    QPoint m_select_start;
    QRect m_select_box;
    std::unique_ptr<QRubberBand> m_rubber_band;
};

#endif //MINOTAUR_CPP_GRIDDISPLAY_H
