#pragma once

#include <vector>
#include "model.hpp"
#include "light.hpp"

class Scene {
public:
    std::vector<Model *> models;
    std::vector<Light *> lights;


    void add(Model *model);

    void add(Light *light);
};