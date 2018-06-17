#ifndef MINOTAUR_CPP_OBJECTMOVE_H
#define MINOTAUR_CPP_OBJECTMOVE_H

#include "../controller/controller.h"
#include "../utility/rect.h"

#include "readymove.h"

#include <QBasicTimer>
#include <QObject>

class StatusLabel;

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
        nrg::dir dir,
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

    void do_move(double delta);
    void correct(double delta);

    /**
     * Whether the object is at or exceeded the target value.
     *
     * @param obj_loc the object's current center location
     * @return true if the object is past the target
     */
    bool beyond_target(const vector2d &obj_loc) const;
    /**
     * Whether the robot is on the correct side of the object
     * for pushing in the desired direction.
     *
     * @param rob_loc the robot's center location
     * @param obj_loc the object's center location
     * @return true if the robot is on the correct side
     */
    bool robot_correct_side(const vector2d &rob_loc, const vector2d &obj_loc) const;
    /**
     * Whether the object's location is beyond the acceptable
     * normal deviation value.
     *
     * @param obj_loc the object's center location
     * @return true if the object has exceeded the norm
     */
    bool exceeded_norm(const vector2d &obj_loc) const;
    /**
     * Determine the robot's alignment error the object's location
     * in the axis perpendicular to the axis of movement. I.e. if
     * the object is to be moved horizontally, this returns the
     * difference in y values of the robot and object.
     *
     * @param rob_loc the robot's center location
     * @param obj_loc the object's center location
     * @return the difference in alignment
     */
    double alignment_err(const vector2d &rob_loc, const vector2d &obj_loc) const;
    /**
     * @param obj_loc the object's center location
     * @return distance to the target location
     */
    double target_err(const vector2d &obj_loc) const;

    std::weak_ptr<Controller> m_sol;
    nrg::dir m_dir;
    double m_target;
    double m_norm_base;
    double m_norm_dev;

    bool m_done;
    /**
     * The stop condition, which is OKAY unless the ObjectMove
     * is completed. When is_done() == true, this value is set
     * to the reason for stopping.
     */
    Stop m_stop;

    StatusLabel *m_align_label;
    StatusLabel *m_target_label;

    QBasicTimer m_timer;

    /**
     * Delegate procedure which is used to fire the controller.
     */
    Procedure m_delegate;
};

#endif //MINOTAUR_CPP_OBJECTMOVE_H
