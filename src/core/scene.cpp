#include "scene.hpp"

void Scene::add(Model *model) {
    models.push_back(model);
}

void Scene::add(Light *light) {
    lights.push_back(light);
}