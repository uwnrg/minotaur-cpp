#ifndef MINOTAUR_CPP_CAMERATHREAD_H
#define MINOTAUR_CPP_CAMERATHREAD_H

#include <QThread>

class IThread final : public QThread {
public:
    ~IThread() override;
};

#endif //MINOTAUR_CPP_CAMERATHREAD_H
