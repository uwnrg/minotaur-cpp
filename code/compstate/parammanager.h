#ifndef MINOTAUR_CPP_PARAMMANAGER_H
#define MINOTAUR_CPP_PARAMMANAGER_H

#include "../gui/mainwindow.h"
#include "../gui/parameterslot.h"

#include <QObject>

#define MANAGE_PARAM(paramType, paramName, defValue)            \
public:                                                         \
volatile paramType paramName = defValue;                        \
private:                                                        \
weak_ref<ParameterSlot> paramName##_slot{nullptr};              \
inline void obtain_##paramName##_slot(parent_t p) {             \
    if (auto lp = p->param_box().lock()) {                      \
        paramName##_slot = lp->add_slot(                        \
            #paramName,                                         \
            QVariant::fromValue(defValue)                       \
        );                                                      \
    }                                                           \
}                                                               \
inline void release_##paramName##_slot(parent_t p) {            \
    if (auto lp = p->param_box().lock()) {                      \
        lp->remove_slot(paramName##_slot);                      \
    }                                                           \
}                                                               \
inline Q_SLOT void set_##paramName(const QVariant &val) {       \
    paramName = val.value<paramType>();                         \
}

#define PARAM_INIT(paramName)           \
obtain_##paramName##_slot(m_p);         \
connect(                                \
    paramName##_slot.get(),             \
    &ParameterSlot::value_set,          \
    this,                               \
    &param_manager::set_##paramName     \
);

#define PARAM_DEINIT(paramName) \
release_##paramName##_slot(m_p);

class param_manager : public QObject {
Q_OBJECT

    typedef MainWindow *parent_t;
    parent_t m_p;

public:
    // CompetitionState
    MANAGE_PARAM(double,  robot_calib_area, 400.0)
    MANAGE_PARAM(double, object_calib_area, 400.0)
    MANAGE_PARAM(double,  area_acq_r_sigma,  1.34)

    // Procedure
    MANAGE_PARAM(int, timer_fast,  50)
    MANAGE_PARAM(int, timer_reg,  200)

    // ObjectProcedure
    MANAGE_PARAM(double, objline_move_dev,  10.0)
    MANAGE_PARAM(double, objmove_algn_err,   2.0)
    MANAGE_PARAM(double, objproc_norm_dev,   4.0)
    MANAGE_PARAM(double, objproc_loc_acpt,   3.0)

    // AStar
    MANAGE_PARAM(int, wall_penalty_0, 10)
    MANAGE_PARAM(int, wall_penalty_1,  5)
    MANAGE_PARAM(int, wall_penalty_2,  2)

public:
    inline explicit param_manager(parent_t p) :
        m_p(p) {
        // CompetitionState
        PARAM_INIT( robot_calib_area)
        PARAM_INIT(object_calib_area)
        PARAM_INIT( area_acq_r_sigma)

        // Procedure
        PARAM_INIT(timer_fast)
        PARAM_INIT(timer_reg )

        // ObjectProcedure
        PARAM_INIT(objline_move_dev)
        PARAM_INIT(objmove_algn_err)
        PARAM_INIT(objproc_norm_dev)
        PARAM_INIT(objproc_loc_acpt)
    }

    inline ~param_manager() override {
        // CompetitionState
        PARAM_DEINIT( robot_calib_area)
        PARAM_DEINIT(object_calib_area)
        PARAM_DEINIT( area_acq_r_sigma)

        // Procedure
        PARAM_DEINIT(timer_fast)
        PARAM_DEINIT(timer_reg )

        // ObjectProcedure
        PARAM_DEINIT(objline_move_dev)
        PARAM_DEINIT(objmove_algn_err)
        PARAM_DEINIT(objproc_norm_dev)
        PARAM_DEINIT(objproc_loc_acpt)
    }
};

extern param_manager *g_pm;

#endif //MINOTAUR_CPP_PARAMMANAGER_H
