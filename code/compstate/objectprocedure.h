#ifndef MINOTAUR_CPP_OBJECTPROCEDURE_H
#define MINOTAUR_CPP_OBJECTPROCEDURE_H

#include <QObject>
#include <memory>

namespace nrg {
    template<typename val_t> class vector;
}
class Controller;
class ObjectLine;
class StatusLabel;
typedef std::vector<nrg::vector<double>> path2d;

/**
 * This procedure combines ObjectLine procedures to move the object
 * in a series of rectangular paths.
 */
class ObjectProcedure : public QObject {
Q_OBJECT

public:
    explicit ObjectProcedure(std::weak_ptr<Controller> sol, const path2d &path);
    ~ObjectProcedure() override;

    void start();
    void stop();

    bool is_done();

private:
    void timerEvent(QTimerEvent *ev) override;

    void movement_loop();

    class Impl;
    std::unique_ptr<Impl> m_impl;
    std::weak_ptr<Controller> m_sol;

    bool m_done;
    bool m_start;

    StatusLabel *m_index_label;
    std::unique_ptr<ObjectLine> m_object_line;
};

#endif //MINOTAUR_CPP_OBJECTPROCEDURE_H
