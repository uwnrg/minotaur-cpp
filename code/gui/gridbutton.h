#ifndef MINOTAUR_CPP_GRIDBUTTON_H
#define MINOTAUR_CPP_GRIDBUTTON_H

#include <QPushButton>
#include <QRubberBand>

#include <memory>

class GridDisplay;

class GridButton : public QPushButton {
Q_OBJECT

public:
    explicit GridButton(GridDisplay *grid_display);

private:
    void mousePressEvent(QMouseEvent *ev) override;

    void mouseReleaseEvent(QMouseEvent *ev) override;

    void mouseMoveEvent(QMouseEvent *ev) override;

    GridDisplay *m_grid_display;

    bool mouse_clicked = false;

    std::unique_ptr<QRubberBand> m_rubber_band;

};


#endif //MINOTAUR_CPP_GRIDBUTTON_H
