#pragma once

#include <vector>

#include "world/baseEntity.hpp"

class World
{
public:
    std::vector<BaseEntity *> entities;

    World();

    void update();
    void addEntity(BaseEntity *e);
    std::vector<mesh> render();
};
