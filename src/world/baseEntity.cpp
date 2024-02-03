#include "world/baseEntity.hpp"

#include <stdexcept>

#include "debug.hpp"
#include "game.hpp"

BaseEntity::BaseEntity(Game *game, bool gravity)
{
    this->velocity = {0, 0, 0};
    this->game = game;
    this->accel = DEFAULT_ACCEL;
    this->maxSpeed = MAX_SPEED;
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

float BaseEntity::getYaw()
{
    return 0;
}

float BaseEntity::getPitch()
{
    return 0;
}
void BaseEntity::setYaw(float yaw)
{
    return;
}
void BaseEntity::setPitch(float pitch)
{
    return;
}

bool BaseEntity::canMove(vec3 v)
{
    mesh m;
    vec3 collisionPoint;

    for (BaseEntity *e : game->world.entities)
    {
        m = e->render();

        for (renderTriangle rt : m)
        {
            collisionPoint = linePlaneCollision(triangleNormal(rt.tri), rt.tri.p1, getPos(), addVec3(getPos(), v));

            if (vec3eq(collisionPoint, {0, 0, 0}))
                continue;  // No collision

            if (inTriangle(projTriangle({{}, 1}, rt.tri), projVec({{}, 1}, collisionPoint)))
                return false;
        }
    }

    return true;
}

void BaseEntity::update()
{
    updateVelocity();
    updatePos();
};
