#include "Scene.h"

Scene::Scene() : Actor(nullptr) {
    camera = Camera::create();
    camera->move({2, 3, -5});

    light = Light::create();
    light->move({3, 5, -2});
}

ScenePtr Scene::create(){
    return ScenePtr(new Scene());
}

void Scene::render(){
    Mesh::setViewMat(camera->getViewMat());
    Mesh::setProjectionMat(camera->getProjectionMat());
    Mesh::setLightPosition(light->getWorldPosition());
    Actor::render(glm::mat4(1.0f));
}