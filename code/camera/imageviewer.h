#ifndef MINOTAUR_CPP_IMAGEVIEWER_H_H
#define MINOTAUR_CPP_IMAGEVIEWER_H_H

#include <QObject>
#include <QImage>
#include <QWidget>

#include <opencv2/opencv.hpp>

class QPaintEvent;

class ImageViewer : public QWidget {
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = nullptr);

    const QImage &getImage();

    Q_SLOT void setImage(const QImage &img);

private:
    void paintEvent(QPaintEvent *) override;

    QImage m_img;
};

#endif //MINOTAUR_CPP_IMAGEVIEWER_H_H
