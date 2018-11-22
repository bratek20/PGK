#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"

class Scene;
using ScenePtr = std::shared_ptr<Scene>;

class Scene : public Actor {
    CameraPtr camera;

public:
    static ScenePtr create();
    void render();

protected:
    Scene();
};
#endif