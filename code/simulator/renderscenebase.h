#ifndef MINOTAUR_CPP_RENDERSCENEBASE_H
#define MINOTAUR_CPP_RENDERSCENEBASE_H

#define MU_0f ((float) M_PI * 4e-7f)
#define MU_sf 15.0f
#define gf 9.809f

#include "drawable.h"

class Solenoid;

class RenderSceneBase {
public:
    typedef typename Drawable::vector2f vector2f;

    virtual vector2f center() const = 0;
    virtual const std::vector<Solenoid> *solenoids() const = 0;
};

#endif //MINOTAUR_CPP_RENDERSCENEBASE_H
