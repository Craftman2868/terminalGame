#pragma once

#include "config.hpp"

#include "3d/maths.hpp"
#include "3d/model.hpp"

// #include "game.hpp"

class Game;

enum EntityType
{
    WORLD_ELEMENT,
    PLAYER,
} __attribute__((__packed__));

class BaseEntity
{
protected:
    Game *game;
    EntityType type;
    float accel;
    vec3 velocity;
    unsigned char life;
    unsigned char max_life;
    bool gravity;

public:
    BaseEntity(Game *game, bool gravity);
    BaseEntity(Game *game);

    // Moves
    virtual vec3 getPos();
    virtual void setPos(vec3 pos);
    void move(vec3 v) { setPos(addVec3(getPos(), v)); }

    virtual double getYaw();
    virtual double getPitch();

    virtual void setYaw(double yaw);
    virtual void setPitch(double pitch);

    void setDirection(double yaw, double pitch)
    {
        setYaw(yaw);
        setPitch(pitch);
    }

    void lookAtLeft(float n) { setYaw(getYaw() + n); }
    void lookAtRight(float n) { setYaw(getYaw() - n); }
    void lookAtUp(float n) { setPitch(getPitch() + n); }
    void lookAtDown(float n) { setPitch(getPitch() - n); }

    void lookAtLeft() { lookAtLeft(DIRECTION_SPEED); }
    void lookAtRight() { lookAtRight(DIRECTION_SPEED); }
    void lookAtUp() { lookAtUp(DIRECTION_SPEED); }
    void lookAtDown() { lookAtDown(DIRECTION_SPEED); }

    vec3 getForwardDirection() { return {-std::sin(getYaw()), 0, std::cos(getYaw())}; }
    vec3 getRightDirection() { return {std::cos(getYaw()), 0, std::sin(getYaw())}; }

    double getSpeed() { return lengthVec3(velocity); }
    void setSpeed(double speed) { velocity = mulVec3(normalizeVec3(velocity), speed); }

    float getAccel() { return accel; }
    void setAccel(float accel) { this->accel = accel; };

    void go(vec3 v)
    {
        addVec3(&velocity, v);

        if (getSpeed() > MAX_SPEED)
            setSpeed(MAX_SPEED);
    }

    void goForward(float accel) { go(mulVec3(getForwardDirection(), accel)); }
    void goRight(float accel) { go(mulVec3(getRightDirection(), accel)); }
    void goBack(float accel) { go(mulVec3(getForwardDirection(), -accel)); }
    void goLeft(float accel) { go(mulVec3(getRightDirection(), -accel)); }
    void goUp(float accel) { go({0, accel, 0}); }
    void goDown(float accel) { go({0, -accel, 0}); }

    void goForward() { goForward(accel); }
    void goRight() { goRight(accel); }
    void goBack() { goBack(accel); }
    void goLeft() { goLeft(accel); }
    void goUp() { goUp(accel); }
    void goDown() { goDown(accel); }

    // Render
    virtual mesh render()
    {
        return mesh();
    };

    // Update
    virtual void updateVelocity()
    {
        if (gravity)
        {
            goDown(GRAVITY);
        }

        mulVec3(&velocity, (double)1 / (1 + GRAVITY));
    }
    virtual void updatePos() { move(velocity); };

    virtual void update();
};
