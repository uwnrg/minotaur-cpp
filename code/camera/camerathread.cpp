#include "camerathread.h"

IThread::~IThread() {
    quit();
    wait();
}
