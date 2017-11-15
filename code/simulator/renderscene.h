#ifndef MINOTAUR_CPP_RENDERSCENE_H
#define MINOTAUR_CPP_RENDERSCENE_H

#include <QOpenGLWidget>
#include <QVector2D>
#include <QPoint>
#include <QTimer>
#include <memory>

#include "../controller/simulator.h"
#include "renderscenebase.h"
#include "drawable.h"
#include "solenoid.h"
#include "sam.h"

class RenderScene : public QOpenGLWidget, public RenderSceneBase {
Q_OBJECT

public:
    explicit RenderScene(std::shared_ptr<Simulator> simulator, QWidget *parent = nullptr);

    ~RenderScene() override;

protected Q_SLOTS:

    void animate();

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    vector2f center() const override;

    const std::vector<Solenoid> *solenoids() const override;

    void startRender();
    void stopRender();

private:
    QTimer m_timer;
    long long int m_elapsed;

    std::shared_ptr<Simulator> m_simulator;
    std::vector<Solenoid> m_solenoids;
    Sam m_sam;
};


#endif //MINOTAUR_CPP_RENDERSCENE_H
