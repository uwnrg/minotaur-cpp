#ifndef MINOTAUR_CPP_OBJECTMOVE_H
#define MINOTAUR_CPP_OBJECTMOVE_H

#include <QObject>
#include <memory>

// Forward declarations
namespace nrg {
    template<typename val_t> class vector;
}
class Controller;
class StatusLabel;
typedef nrg::vector<double> vector2d;

/**
 * This class moves the robot directly, through a delegate Procedure object,
 * and performs naive movement of the object with the robot. This procedure
 * will attempt to move the robot in a certain direction to push the object
 * in that direction, while maintaining the robot's alignment with the object.
 *
 * This procedure may stop with various stop conditions that indicate whether
 * the procedure failed or reached its target. OKAY is the running value,
 * AT_TARGET indicates that the ObjectMove has pushed the object past its target
 * value, EXCEEDED_NORM means that object's alignment to its rectangular movement
 * line has exceeded a value and needs to be corrected, and WRONG_SIDE means the
 * robot has moved too far beyond the object, and needs another ReadyMove.
 *
 * Creators of this object pass a direction, target value, normal base value,
 * and normal deviation value. The target value is the desired x or y value
 * of the object, where the direction is the move direction. For instance
 * nrg::dir::RIGHT and target = 50 means move right until x >= 50.
 *
 * The normal base value and normal deviation values are to ensure alignment of
 * the object. If the object is moving in the x-direction, normal base value
 * is the initial y-value, and normal deviation is the maximum difference to this.
 */
class ObjectMove : public QObject {
Q_OBJECT

public:
    /**
     * Stop condition values.
     */
    enum Stop {
        OKAY,
        AT_TARGET,
        EXCEEDED_NORM,
        WRONG_SIDE
    };

    /**
     * Create a new ObjectMove procedure.
     *
     * @param sol       reference to controller passed to delegate procedure
     * @param dir       desired movement direction of the object
     * @param target    the target x or y value for the object
     * @param norm_base the base normal value for object alignment
     * @param norm_dev  the maximum deviation from the normal base value
     */
    explicit ObjectMove(
        std::weak_ptr<Controller> sol,
        int dir,
        double target,
        double norm_base,
        double norm_dev
    );
    ~ObjectMove() override;

    void start();
    void stop();

    bool is_done() const;
    Stop get_stop() const;

private:
    void timerEvent(QTimerEvent *ev) override;

    void movement_loop();

    class Impl;
    std::unique_ptr<Impl> m_impl;
    std::weak_ptr<Controller> m_sol;

    bool m_done;
    /**
     * The stop condition, which is OKAY unless the ObjectMove
     * is completed. When is_done() == true, this value is set
     * to the reason for stopping.
     */
    Stop m_stop;

    StatusLabel *m_align_label;
    StatusLabel *m_target_label;
};

#endif //MINOTAUR_CPP_OBJECTMOVE_H
