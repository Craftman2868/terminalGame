#pragma once

#include "world/baseEntity.hpp"

class Player : public BaseEntity
{
public:
    Player(Game *game);

    virtual vec3 getPos();
    virtual void setPos(vec3 pos);

    virtual double getYaw();
    virtual double getPitch();

    virtual void setYaw(double yaw);
    virtual void setPitch(double pitch);

    virtual mesh render();
};
