#ifndef MINOTAUR_CPP_SOLENOID_H
#define MINOTAUR_CPP_SOLENOID_H

#include <QVector2D>
#include <QPainter>
#include <QPaintEvent>

#include "drawable.h"
#include "renderscenebase.h"

class Solenoid : public Drawable {
public:
    Solenoid() = default;
    Solenoid(const RenderSceneBase *scene,
             const vector2f &P, float theta, float n, float R, float l, float mu_rel);

    vector2f fieldAt(const vector2f &Q) const;
    void setCurrent(float i);
    float getCurrent();

    void draw(QPainter *painter, QPaintEvent *, int, float scale) override ;

private:
    vector2f m_pos;
    float m_theta;
    float m_N;
    /**
     * Solenoid radius in meters.
     */
    float m_radius;
    /**
     * Solenoid length in meters.
     */
    float m_len;
    float m_I;
    float m_mu;

    const RenderSceneBase *m_renderScene;

    float getA(float x, float y) const;
};


#endif //MINOTAUR_CPP_SOLENOID_H
