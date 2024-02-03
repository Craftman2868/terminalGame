#include "world/entity.hpp"

Entity::Entity(Game *game, EntityType type, vec3 pos, float yaw, float pitch) : BaseEntity(game)
{
    this->accel = DEFAULT_ACCEL;
    this->type = type;
    this->pos = pos;
    this->yaw = yaw;
    this->pitch = pitch;
    this->model = mesh();
    this->visible = true;
}

Entity::Entity(Game *game, EntityType type, vec3 pos) : Entity(game, type, pos, 0, 0)
{
}

void Entity::setModel(mesh model)
{
    this->model = model;
}

mesh Entity::render() {
    if (!visible)
        return mesh();

    return moveMesh(model, getPos());
}
