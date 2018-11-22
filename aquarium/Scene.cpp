#include "Scene.h"

Scene::Scene() : Actor(nullptr) {
    camera = Camera::create();
    camera->move(glm::vec3(2,3,-5));
}

ScenePtr Scene::create(){
    return ScenePtr(new Scene());
}

void Scene::render(){
    Actor::render(camera->getCameraMat());
}