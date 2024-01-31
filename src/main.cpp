#include <unistd.h>  // usleep
#include <stdio.h>

#include "debug.hpp"

#include "input.hpp"
#include "3d/maths.hpp"
#include "3d/render.hpp"
#include "3d/model.hpp"
#include "world/player.hpp"
#include "game.hpp"
#include "world/entity.hpp"

#define DEFAULT_SPEED 0.03
#define DIRECTION_SPEED 0.02


bool running = false;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s file.obj\n", argv[0]);

        return 1;
    }

    mesh m;

    debug_init();

    debug_log("Loading model...\n");

    m = loadModel(argv[1]);

    if (m.size() == 0)
    {
        debug_quit();
        return 1;
    }

    debug_log("Model loaded (%u triangles)\n", m.size());

    Game game;

    Entity world_mesh = Entity(&game, WORLD_ELEMENT, {0, 0, 0});

    world_mesh.setModel(m);

    game.world.addEntity(&world_mesh);

    game.setLight({{-1, 2, 0}});

    debug_log("Running the game...\n");

    game.run();

    debug_log("Quit\n");

    debug_quit();

    return 0;
}
