#ifndef MINOTAUR_CPP_RENDERSCENE_H
#define MINOTAUR_CPP_RENDERSCENE_H

#include "renderscenebase.h"
#include "simsolenoid.h"
#include "sam.h"

#include <QOpenGLWidget>
#include <QTimer>
#include <memory>

/**
 * Render scene which is where entities are to be drawn.
 * This class handles the real-time updates and rendering.
 */
class RenderScene : public QOpenGLWidget, public RenderSceneBase {
Q_OBJECT

public:
    /**
     * Create a render scene.
     *
     * @param simulator a reference to the simulator instance, used to get key inputs
     * @param parent    parent widget, which should be the simulator
     */
    explicit RenderScene(std::shared_ptr<Simulator> simulator, QWidget *parent = nullptr);

    /**
     * Clean up the render scene, stopping the timer.
     */
    ~RenderScene() override;

protected Q_SLOTS:
    /**
     * Perform an animation update from a ticking timer.
     * Emitted should be a timer. Function grabs the
     * change in time since the last tick and fires update
     * events on all drawn entities.
     */
    void animate();

protected:
    /**
     * Update and draw all entities.
     *
     * @param event paint event
     */
    void paintEvent(QPaintEvent *event) override;

public:
    /**
     * @return the real pixel position (x, y) of the origin (0, 0)
     */
    vector2f center() const override;

    /**
     * @return a vector of solenoids in the field
     */
    const std::vector<SimulatorSolenoid> *solenoids() const override;

    /**
     * @return a pointer to the Sam instance.
     */
    const SAMRobot *sam() const override;

    /**
     * Start the ticker.
     */
    void startRender();
    /**
     * Stop the ticker.
     */
    void stopRender();

private:
    /**
     * Ticker which fires update and draw events.
     */
    QTimer m_timer;
    /**
     * Amount of time elapsed since last update, stored
     * as a member variable.
     */
    long long int m_elapsed;

    /**
     * Reference to the Simulator instance.
     */
    std::shared_ptr<Simulator> m_simulator;
    /**
     * List of solenoids in the field.
     */
    std::vector<SimulatorSolenoid> m_solenoids;
    /**
     * Object representing the location of SAM.
     */
    SAMRobot m_sam;
};


#endif //MINOTAUR_CPP_RENDERSCENE_H
