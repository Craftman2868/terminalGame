#pragma once

#include "config.hpp"

#include "renderer.hpp"
#include "world/player.hpp"
#include "world/world.hpp"

#include <chrono>

#define MS_PER_TICK ((milliseconds)(1000 / MAX_TPS))

using namespace std::chrono;

class Game
{
private:
    Renderer renderer;
    Player pl;
    bool running;
    milliseconds last_tick;

public:
    World world;
    camera *cam;

    Game();

    lightSource getLight() { return renderer.getLight(); }
    void setLight(lightSource light) { renderer.setLight(light); }

    void tick();

    void check_input();
    bool update();
    void render();

    void run();
};
