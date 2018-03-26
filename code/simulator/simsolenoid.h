#ifndef MINOTAUR_CPP_SOLENOID_H
#define MINOTAUR_CPP_SOLENOID_H

#include <QVector2D>
#include <QPainter>
#include <QPaintEvent>

#include "drawable.h"
#include "renderscenebase.h"

/**
 * SimulatorSolenoid class represents an electromagnetic that is
 * passed a current @code i @endcode to produce a magnetic
 * field that moves SAM.
 */
class SimulatorSolenoid : public Drawable {
public:
    SimulatorSolenoid() = default;
    /**
     * Create a solenoid.
     *
     * @param scene  pointer to the render scene
     * @param P      the location of the center of the solenoid
     * @param theta  the rotation of the solenoid from the positive x axis
     * @param n      the number of coils in the solenoid
     * @param R      the radius of the solenoid
     * @param l      the length of the solenoid
     * @param mu_rel the relative permeability of the solenoid
     */
    SimulatorSolenoid(const RenderSceneBase *scene,
             const vector2f &P, float theta, float n, float R, float l, float mu_rel);

    /**
     * Get the magnetic field produced by this solenoid.
     *
     * @param Q the position to find the field
     * @return the magnetic field vector at that position
     */
    vector2f fieldAt(const vector2f &Q) const;
    /**
     * Set the current passing through the solenoid,
     * which influences the produced magnetic field. Current of zero
     * turns off the solenoid.
     *
     * @param i current of the solenoid
     */
    void setCurrent(float i);
    /**
     * @return the current of the solenoid
     */
    float getCurrent();

    /**
     * Draw the solenoid.
     *
     * @param painter the painter to use
     * @param scale   the scale to use (pixels / m)
     */
    void draw(QPainter *painter, QPaintEvent *, int, float scale) override ;

private:
    /**
     * Position of the solenoid in meters from the origin.
     */
    vector2f m_pos;
    /**
     * Angle of the solenoid from the positive x-axis in radians
     */
    float m_theta;
    /**
     * Number of coil rotations.
     */
    float m_N;
    /**
     * SimulatorSolenoid radius in meters.
     */
    float m_radius;
    /**
     * SimulatorSolenoid length in meters.
     */
    float m_len;
    /**
     * Current passing through the solenoid (A).
     */
    float m_I;
    /**
     * Relative permeability (unit-less).
     */
    float m_mu;

    /**
     * Pointer to the reference scene.
     */
    const RenderSceneBase *m_renderScene;

    /**
     * Helper scene for computing magnetic fields.
     *
     * @param x the x-coordinate
     * @param y the y-coordinate
     * @return coefficient
     */
    float getA(float x, float y) const;
};


#endif //MINOTAUR_CPP_SOLENOID_H
