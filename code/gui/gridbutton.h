#ifndef MINOTAUR_CPP_GRIDBUTTON_H
#define MINOTAUR_CPP_GRIDBUTTON_H

#include <QPushButton>

class GridDisplay;

class GridButton : public QPushButton {
Q_OBJECT

public:
    explicit GridButton(GridDisplay *grid_display);

    bool mouse_is_clicked();
protected:
    ~GridButton();

private:
    void mousePressEvent(QMouseEvent *ev) override;

    void mouseReleaseEvent(QMouseEvent *ev) override;

    void mouseMoveEvent(QMouseEvent *ev) override;

    void paintEvent(QPaintEvent *ev) override;

    GridDisplay *m_grid_display;

    bool mouse_clicked = false;

};


#endif //MINOTAUR_CPP_GRIDBUTTON_H
