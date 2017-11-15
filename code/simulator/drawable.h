#ifndef MINOTAUR_CPP_DRAWABLE_H
#define MINOTAUR_CPP_DRAWABLE_H

#include <cmath>

#include <QPaintEvent>

class Drawable {
public:
    typedef QVector2D vector2f;
    typedef QPointF pointf;

    static inline vector2f rotate(const vector2f &Q, float theta) {
        float Qpx = Q.x() * cosf(theta) - Q.y() * sinf(theta);
        float Qpy = Q.y() * cosf(theta) + Q.x() * sinf(theta);
        return {Qpx, Qpy};
    }

    static inline vector2f translate(const vector2f &Q, const vector2f &P) {
        return {Q.x() - P.x(), Q.y() - P.y()};
    }

    virtual void draw(QPainter *, QPaintEvent *, int, float) = 0;
};

#endif //MINOTAUR_CPP_DRAWABLE_H
