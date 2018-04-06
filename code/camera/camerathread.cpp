#include "camerathread.h"

IThread::~IThread() {
    // Do not want to destroy resources in the thread
    // until the program is over
    quit();
    wait();
}
