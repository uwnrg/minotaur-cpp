#ifndef MINOTAUR_CPP_STATSDISPLAY_H
#define MINOTAUR_CPP_STATSDISPLAY_H

#include "drawable.h"
#include "renderscenebase.h"

class StatsDisplay : public Drawable {
public:
    explicit StatsDisplay(RenderSceneBase *render_scene);

    void draw(QPainter *painter, QPaintEvent *, int, float) override;

private:
    RenderSceneBase *m_render_scene;
};

#endif //MINOTAUR_CPP_STATSDISPLAY_H
