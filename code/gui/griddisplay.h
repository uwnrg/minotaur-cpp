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
        DEFAULT_WEIGHT = 0,
        GRID_SIZE = 20,
        SCENE_WIDTH = 100,      // Default: 800
        SCENE_HEIGHT = 100,     // Default: 400
    };

    struct Coord {
        int x;
        int y;
    };

    GridDisplay(ImageViewer *image_viewer, CameraDisplay *camera_display);

    ~GridDisplay() override;

    void mousePressEvent(QMouseEvent *ev) override;

    void mouseReleaseEvent(QMouseEvent *ev) override;

    void mouseMoveEvent(QMouseEvent *ev) override;

    void paintEvent(QPaintEvent *ev) override;

    void set_coordinates(Coord& coord, const int x, const int y);

    void set_coordinates(Coord& coord, const QPoint& pos);

    void set_mouse_start(const QPoint& pos);

    void set_mouse_release(const QPoint& pos);

    int get_row_count();

    int get_column_count();

public Q_SLOTS:
    void clear_selection();

    void show_grid();

    void hide_grid();

    //TODO: figure out refactoring conflict
    void selectRobotPosition(QString);

protected Q_SLOTS:
    void button_clicked(int x, int y);

private:
    void show_view();

    void update_scene();

    void draw_grid();

    void draw_buttons();

    void init_start_end_pos();

    void mouse_clicked();

    void rect_select_buttons(const Coord top_left, const Coord bottom_right);

    std::unique_ptr<QGraphicsScene> m_scene;
    std::unique_ptr<QGraphicsView> m_view;

    CameraDisplay *m_camera_display;

    GridButton *m_button[40][20];
    array2d<int> m_square_selected {40, 20};

    int m_column_count = SCENE_WIDTH / GRID_SIZE;   // Default: 40
    int m_row_count = SCENE_HEIGHT / GRID_SIZE;     // Default: 20
    bool m_grid_displayed = false;

    bool m_start_pos_selected = false;
    bool m_end_pos_selected = false;

    Coord m_start_position;
    Coord m_end_position;

    Coord m_mouse_click_start;
    Coord m_mouse_click_release;

    QPoint select_start;
    QRect select_box;
    QRubberBand *rubber_band;
};

#endif //MINOTAUR_CPP_GRIDDISPLAY_H
