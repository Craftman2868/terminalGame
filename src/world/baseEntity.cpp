#include "world/baseEntity.hpp"

#include <stdexcept>

#include "debug.hpp"

BaseEntity::BaseEntity(Game *game, bool gravity)
{
    this->velocity = {0, 0, 0};
    this->game = game;
    this->accel = DEFAULT_ACCEL;
    this->gravity = gravity;
}

BaseEntity::BaseEntity(Game *game) : BaseEntity(game, false)
{
}

vec3 BaseEntity::getPos()
{
    return {0, 0, 0};
}

void BaseEntity::setPos(vec3 pos)
{
    return;
}

double BaseEntity::getYaw()
{
    return 0;
}

double BaseEntity::getPitch()
{
    return 0;
}
void BaseEntity::setYaw(double yaw)
{
    return;
}
void BaseEntity::setPitch(double pitch)
{
    return;
}

void BaseEntity::update()
{
    updateVelocity();
    updatePos();
};
