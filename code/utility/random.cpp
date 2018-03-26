#include "random.h"

std::mt19937 &rng::engine() {
    thread_local static std::random_device rd;
    thread_local static std::mt19937 engine(rd());
    return engine;
}

