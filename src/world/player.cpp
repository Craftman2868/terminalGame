#include "world/player.hpp"

#include "world/baseEntity.hpp"
#include "game.hpp"

Player::Player(Game *game) : BaseEntity(game)
{
    this->type  = PLAYER;
    this->accel = DEFAULT_ACCEL;
}
vec3 Player::getPos()
{
    return game->cam->pos;
}
void Player::setPos(vec3 pos)
{
    game->cam->pos = pos;
}

float Player::getYaw()
{
    return game->cam->yaw;
}

float Player::getPitch()
{
    return game->cam->pitch;
}

void Player::setYaw(float yaw)
{
    game->cam->yaw = yaw;
}

void Player::setPitch(float pitch)
{
    game->cam->pitch = pitch;
}

mesh Player::render()
{
    return mesh();
}
