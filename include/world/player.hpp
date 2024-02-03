#pragma once

#include "world/baseEntity.hpp"

class Player : public BaseEntity
{
public:
    Player(Game *game);

    virtual vec3 getPos();
    virtual void setPos(vec3 pos);

    virtual float getYaw();
    virtual float getPitch();

    virtual void setYaw(float yaw);
    virtual void setPitch(float pitch);

    virtual mesh render();
};
