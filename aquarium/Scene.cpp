#include "Scene.h"

Scene::Scene() : Actor(nullptr) {
    camera = Camera::create();

    light = Light::create();
    light->move({3, 5, -2});
}

ScenePtr Scene::create(){
    ScenePtr scene = ScenePtr(new Scene());
    scene->addChilds({scene->camera, scene->light});
    return scene; 
}

void Scene::render(){
    Mesh::setViewMat(camera->getViewMat());
    Mesh::setProjectionMat(camera->getProjectionMat());
    Mesh::setLightPosition(light->getWorldPosition());
    Actor::render(glm::mat4(1.0f));
}

CameraPtr Scene::getCamera() const{
    return camera;
}