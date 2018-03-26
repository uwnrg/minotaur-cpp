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

#include "../utility/array2d.h"
#include "../utility/utility.h"
#include "../code/camera/cameradisplay.h"
#include "../code/camera/imageviewer.h"
#include "../code/utility/array2d.h"
#include "../code/utility/utility.h"

class CameraDisplay;

class GridDisplay : public QWidget {
Q_OBJECT

public:
    GridDisplay(ImageViewer *image_viewer, CameraDisplay *camera_display);

    ~GridDisplay() override;

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

    std::unique_ptr<QGraphicsScene> m_scene;
    std::unique_ptr<QGraphicsView> m_view;

    CameraDisplay *m_camera_display;

    QPushButton *m_button[40][20];
    array2d<int> m_square_selected {40, 20};

    const int m_grid_size = 20;
    const int m_scene_width = 100;                      // Default: 800
    const int m_scene_height = 100;                     // Default: 400
    int m_column_count = m_scene_width / m_grid_size;   // Default: 40
    int m_row_count = m_scene_height / m_grid_size;     // Default: 20
    bool m_grid_displayed = false;

    const int m_not_selected_weight = -1;
    const int m_default_weight = 0;
    const int m_start_weight = -2;
    const int m_end_weight = -3;

    bool m_start_pos_selected = false;
    bool m_end_pos_selected = false;

    struct Coord {
        int x;
        int y;
    };

//    std::unique_ptr<Coord> m_start_position;
//    std::unique_ptr<Coord> m_end_position;
    Coord m_start_position;
    Coord m_end_position;
};

#endif //MINOTAUR_CPP_GRIDDISPLAY_H
