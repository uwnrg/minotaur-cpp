#include <QDebug>
#include "gridbutton.h"
#include "griddisplay.h"

//create class that extends QPushButton -> replace all QPushButton with your own button name
//subclass QPushButton and then call events on parents

GridButton::GridButton(GridDisplay *grid_display) :
    QPushButton(),
    m_grid_display(grid_display) {
    setMouseTracking(true);
}

void GridButton::mousePressEvent(QMouseEvent *ev) {
    qDebug() << "Mouse pressed: " << this->mapToParent(ev->pos());
    mouse_clicked = true;
    m_grid_display->set_mouse_start(this->mapToParent(ev->pos()));
    m_grid_display->mousePressEvent(ev);
    QPushButton::mousePressEvent(ev);
}

void GridButton::mouseReleaseEvent(QMouseEvent *ev) {
    qDebug() << "Button Mouse released: " << ev->pos();
    mouse_clicked = false;
    m_grid_display->set_mouse_release(this->mapToParent(ev->pos()));
    m_grid_display->mouseReleaseEvent(ev);
    QPushButton::mouseReleaseEvent(ev);
}

void GridButton::mouseMoveEvent(QMouseEvent *ev) {
    if (hasMouseTracking() && mouse_clicked) {
        m_grid_display->mouseMoveEvent(ev);
        QPushButton::mouseMoveEvent(ev);
    }
}

void GridButton::paintEvent(QPaintEvent *ev) {

}

bool GridButton::mouse_is_clicked() {
    return mouse_clicked;
}

GridButton::~GridButton() {

}
