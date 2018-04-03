#ifndef MINOTAUR_CPP_PROCEDURE_H
#define MINOTAUR_CPP_PROCEDURE_H

#include <memory>

#include <QObject>
#include <QBasicTimer>

#include "../utility/vector.h"

#define DEFAULT_TARGET_LOC_ACCEPTANCE 3.5
#define DEFAULT_MAX_NORMAL_DEVIATION  6.5

class Controller;
class StatusLabel;

class Procedure : public QObject {
Q_OBJECT

public:
    Procedure(
        std::weak_ptr<Controller> sol,
        const path2d &path,
        double loc_accept = DEFAULT_TARGET_LOC_ACCEPTANCE,
        double norm_dev = DEFAULT_MAX_NORMAL_DEVIATION
    );

    ~Procedure() override;

    void start();
    void stop();

    bool is_done() const;
    bool is_stopped() const;

    Q_SIGNAL void started();

    Q_SIGNAL void stopped();

    Q_SIGNAL void finished();

    // estimated_power is the positive error in the direction of movement,
    // and it is up to the controller to translate this into a solenoid voltage
    void move_right(double estimated_power);
    void move_left(double estimated_power);
    void move_up(double estimated_power);
    void move_down(double estimated_power);

private:
    void timerEvent(QTimerEvent *ev) override;

    void movement_loop();

    double m_loc_accept;
    double m_norm_dev;
    path2d m_path;
    vector2d m_initial;
    std::size_t m_index;
    std::weak_ptr<Controller> m_sol;
    QBasicTimer m_timer;

    StatusLabel *m_dir_label;
    StatusLabel *m_err_label;
    StatusLabel *m_index_label;
    StatusLabel *m_perp_label;

    bool m_done;
};

#endif //MINOTAUR_CPP_PROCEDURE_H
