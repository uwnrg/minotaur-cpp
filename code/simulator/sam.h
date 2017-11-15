#ifndef MINOTAUR_CPP_SAM_H
#define MINOTAUR_CPP_SAM_H

#include "drawable.h"
#include "renderscenebase.h"

class Sam : public Drawable {
public:
    explicit Sam(RenderSceneBase *scene, float mu_s, float mass, float length);

    void draw(QPainter *paint, QPaintEvent *, int elapsed, float scale) override;

    void stop();

    const vector2f &pos() const;

    const vector2f &mag() const;

private:

    RenderSceneBase *m_renderScene;
    vector2f m_pos;
    vector2f m_vel;
    vector2f m_mag;
    float m_mass;
    float m_len;
    float m_fric;
};


#endif //MINOTAUR_CPP_SAM_H
