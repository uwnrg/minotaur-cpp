#include "gridbutton.h"
#include "griddisplay.h"

#ifndef NDEBUG
#include <QDebug>
#endif

GridButton::GridButton(GridDisplay *grid_display) :
    QPushButton(),
    m_grid_display(grid_display) {
    setMouseTracking(true);
}

void GridButton::mousePressEvent(QMouseEvent *ev) {
#ifndef NDEBUG
        qDebug() << "Mouse pressed: " << mapToParent(ev->pos());
#endif
        mouse_clicked = true;
        m_grid_display->set_mouse_start(mapToParent(ev->pos()));
        m_grid_display->mousePressEvent(ev);
        QPushButton::mousePressEvent(ev);
}

void GridButton::mouseReleaseEvent(QMouseEvent *ev) {
#ifndef NDEBUG
    qDebug() << "Mouse released: " << ev->pos();
#endif
    mouse_clicked = false;
    m_grid_display->set_mouse_release(mapToParent(ev->pos()));
    m_grid_display->mouseReleaseEvent(ev);
    QPushButton::mouseReleaseEvent(ev);
}

void GridButton::mouseMoveEvent(QMouseEvent *ev) {
    if (hasMouseTracking() && mouse_clicked) {
        m_grid_display->set_mouse_move(mapToParent(ev->pos()));
        m_grid_display->mouseMoveEvent(ev);
        QPushButton::mouseMoveEvent(ev);
    }
}
