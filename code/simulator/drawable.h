#ifndef MINOTAUR_CPP_DRAWABLE_H
#define MINOTAUR_CPP_DRAWABLE_H

#include <cmath>

#include <QPaintEvent>

/**
 * This class represents any entity that has a position
 * and can be drawn on a render scene.
 */
class Drawable {
public:
    typedef QVector2D vector2f;
    typedef QPointF pointf;

    /**
     * Rotate a vector by an angle.
     *
     * @param Q     vector to rotate
     * @param theta angle in radians
     * @return a new rotated vector
     */
    static inline vector2f rotate(const vector2f &Q, float theta) {
        float Qpx = Q.x() * cosf(theta) - Q.y() * sinf(theta);
        float Qpy = Q.y() * cosf(theta) + Q.x() * sinf(theta);
        return {Qpx, Qpy};
    }

    /**
     * Translate a vector.
     *
     * @param Q vector to translate
     * @param P vector representing the displacement
     * @return a new translated vector
     */
    static inline vector2f translate(const vector2f &Q, const vector2f &P) {
        return {Q.x() - P.x(), Q.y() - P.y()};
    }

    /**
     * Draw the object.
     */
    virtual void draw(QPainter *, QPaintEvent *, int, float) = 0;
};

#endif //MINOTAUR_CPP_DRAWABLE_H
