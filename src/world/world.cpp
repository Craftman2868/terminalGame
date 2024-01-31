#include "world/world.hpp"

#include <vector>

World::World()
{
}

void World::update()
{
    for (BaseEntity *e : entities)
    {
        e->update();
    }
}

void World::addEntity(BaseEntity *e)
{
    entities.push_back(e);
}

std::vector<mesh> World::render()
{
    std::vector<mesh> m;

    for (BaseEntity *e : entities)
    {
        m.push_back(e->render());
    }

    return m;
}
