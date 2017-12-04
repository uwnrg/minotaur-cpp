#ifndef MINOTAUR_CPP_ARROW_H
#define MINOTAUR_CPP_ARROW_H

// Pixel dimensions for arrows, which adjust display size
#define ARR_LEN 25
#define ARR_X 3
#define ARR_Y 9

#include "drawable.h"
#include "renderscenebase.h"

/**
 * Arrow class represents an arrow that is drawn in
 * the simulator to show the direction of the net
 * magnetic field.
 */
class Arrow : public Drawable {
public:
    /**
     * Create an arrow to be drawn.
     *
     * @param scene render scene on which to draw
     * @param P position vector of the arrow
     * @param B magnetic field vector
     */
    Arrow(RenderSceneBase *scene, const vector2f &P, const vector2f &B);

    /**
     * Draw the arrow.
     *
     * @param painter painter to draw on
     */
    void draw(QPainter *painter, QPaintEvent *, int, float) override;

private:
    RenderSceneBase *m_renderScene;

    vector2f m_pos;
    vector2f m_mag;
};

#endif //MINOTAUR_CPP_ARROW_H
