#include "imageviewer.h"

#include <QPainter>

ImageViewer::ImageViewer(QWidget *parent)
    : QWidget(parent) {
    setAttribute(Qt::WA_OpaquePaintEvent);
}

const QImage &ImageViewer::getImage() {
    return m_img;
}

void ImageViewer::setImage(const QImage &img) {
    m_img = img;
    if (m_img.size() != size()) {
        setFixedSize(m_img.size());
    }
    update();
}

void ImageViewer::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(0, 0, m_img);
}
