#ifndef MINOTAUR_CPP_RANDOM_H
#define MINOTAUR_CPP_RANDOM_H

#include <random>

namespace rng {

    std::mt19937 &engine();

    template<int lower, int upper>
    double randf() {
        thread_local static std::uniform_real_distribution<> dis(lower, upper);
        return dis(engine());
    }

}

#endif

