#pragma once

#include <vector>

#include "world/baseEntity.hpp"

class World
{
private:
    std::vector<BaseEntity *> entities;

public:
    World();

    void update();
    void addEntity(BaseEntity *e);
    std::vector<mesh> render();
};
