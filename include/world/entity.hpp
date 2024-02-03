#pragma once

#include "world/baseEntity.hpp"


class Entity : public BaseEntity
{
protected:
    vec3 pos;
    float yaw, pitch;
    bool visible;
    mesh model;

public:
    Entity(Game *game, EntityType type, vec3 pos, float yaw, float pitch);
    Entity(Game *game, EntityType type, vec3 pos);
    ~Entity() {}

    virtual vec3 getPos() { return pos; }
    virtual void setPos(vec3 pos) { this->pos = pos; }

    virtual float getYaw()   { return yaw; }
    virtual float getPitch() { return pitch; }

    virtual void setYaw(float yaw)     { this->yaw = yaw; }
    virtual void setPitch(float pitch) { this->pitch = pitch; }

    bool getVisible() { return visible; }
    void setVisible(bool visible) { this->visible = visible; };

    void setModel(mesh model);

    virtual mesh render();
};
