#ifndef MINOTAUR_CPP_SAM_H
#define MINOTAUR_CPP_SAM_H

#include "drawable.h"
#include "renderscenebase.h"

/**
 * This class represents SAM, a robot with an orientation and
 * position in the field.
 */
class Sam : public Drawable {
public:
    /**
     * Create an instance of SAM.
     *
     * @param scene  render scene reference
     * @param mu_s   coefficient of static friction for SAM
     * @param mass   the mass of SAM
     * @param length the length of SAM (in meters), which is shaped as a square
     */
    explicit Sam(RenderSceneBase *scene, float mu_s, float mass, float length);

    /**
     * Draw SAM.
     *
     * @param paint   painter
     * @param elapsed amount of time elapsed
     * @param scale   the scale to use when drawing (pixels / m)
     */
    void draw(QPainter *paint, QPaintEvent *, int elapsed, float scale) override;

    /**
     * Zero SAM's velocity.
     */
    void stop();

    /**
     * Reset SAM's position.
     */
    void reset();

    /**
     * @return SAM's current position
     */
    const vector2f &pos() const;

    /**
     * @return SAM's current velocity
     */
    const vector2f &vel() const;

    /**
     * @return the current magnetic field magnitude on SAM
     */
    const vector2f &mag() const;

private:

    /**
     * Render scene reference.
     */
    RenderSceneBase *m_renderScene;
    /**
     * Current position vector.
     */
    vector2f m_pos;
    /**
     * Current velocity vector.
     */
    vector2f m_vel;
    /**
     * Current magnetic field magnitude, stored as a member
     * variable for access by other classes.
     */
    vector2f m_mag;
    /**
     * The mass of this SAM.
     */
    float m_mass;
    /**
     * The length of a side of this SAM.
     */
    float m_len;
    /**
     * Coefficient of state friction on this SAM.
     */
    float m_fric;
};


#endif //MINOTAUR_CPP_SAM_H
