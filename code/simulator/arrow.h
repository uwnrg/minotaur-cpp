#ifndef MINOTAUR_CPP_ARROW_H
#define MINOTAUR_CPP_ARROW_H

#define ARR_LEN 25
#define ARR_X 3
#define ARR_Y 9

#include "drawable.h"
#include "renderscenebase.h"

class Arrow : public Drawable {
public:
    Arrow(RenderSceneBase *scene, const vector2f &P, const vector2f &B);

    void draw(QPainter *painter, QPaintEvent *, int, float) override;

private:
    RenderSceneBase *m_renderScene;

    vector2f m_pos;
    vector2f m_mag;
};

#endif //MINOTAUR_CPP_ARROW_H
