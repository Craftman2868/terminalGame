#pragma once

#include "world/baseEntity.hpp"


class Entity : public BaseEntity
{
protected:
    vec3 pos;
    double yaw, pitch;
    bool visible;
    mesh model;

public:
    Entity(Game *game, EntityType type, vec3 pos, double yaw, double pitch);
    Entity(Game *game, EntityType type, vec3 pos);
    ~Entity() {}

    virtual vec3 getPos() { return pos; }
    virtual void setPos(vec3 pos) { this->pos = pos; }

    virtual double getYaw()   { return yaw; }
    virtual double getPitch() { return pitch; }

    virtual void setYaw(double yaw)     { this->yaw = yaw; }
    virtual void setPitch(double pitch) { this->pitch = pitch; }

    bool getVisible() { return visible; }
    void setVisible(bool visible) { this->visible = visible; };

    void setModel(mesh model);

    virtual mesh render();
};
