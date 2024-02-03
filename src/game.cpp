#include "game.hpp"

#include "input.hpp"
#include "3d/render.hpp"
#include "world/player.hpp"
#include "debug.hpp"

#include <chrono>
#include <thread>

using namespace std::chrono;

milliseconds time_ms()
{
    return duration_cast<milliseconds>(
        system_clock::now().time_since_epoch());
}

Game::Game() : pl(this), renderer((lightSource){{0, 0, 0}})
{
    cam = renderer.cam;
    running = false;

    last_tick = time_ms();

    world.addEntity(&pl);
}

void Game::tick()
{
    render_tick();

    if (time_ms() - last_tick < MS_PER_TICK)
    {
        std::this_thread::sleep_for(MS_PER_TICK - (time_ms() - last_tick));
    }

    last_tick = time_ms();
}

void Game::check_input()
{
    Key key;

    while (getkey_noblock(&key))
    {
        if (key.ctrl && (key.ch == 'C' || key.ch == 'Q'))
        {
            running = false;
            break;
        }

        switch (key.ch)
        {
        case 0:  // special key
            switch (key.key)
            {
            case KEY_DOWN:
                pl.lookAtDown();
                break;
            case KEY_UP:
                pl.lookAtUp();
                break;
            case KEY_LEFT:
                pl.lookAtLeft();
                break;
            case KEY_RIGHT:
                pl.lookAtRight();
                break;
            default:
                break;
            }
            break;
        case 'Z':
            pl.goForward();
            break;
        case 'S':
            pl.goBack();
            break;
        case 'Q':
            pl.goLeft();
            break;
        case 'D':
            pl.goRight();
            break;
        case 'E':
        case ' ':
            pl.goUp();
            break;
        case 'A':
            pl.goDown();
            break;
        case '0':
            pl.setPos({0, 0, 0});
            pl.setDirection(0, 0);
            break;
        case '+':
            if (pl.getAccel() < 0.001)
                pl.setAccel(0.005);
            else if (pl.getAccel() > 10)
                break;
            pl.setAccel(pl.getAccel() * 2);
            pl.setMaxSpeed(pl.getMaxSpeed() * 2);
            break;
        case '-':
            pl.setAccel(pl.getAccel() / 2);
            pl.setMaxSpeed(pl.getMaxSpeed() / 2);
            break;
        case '*':
            pl.setAccel(DEFAULT_ACCEL);
            pl.setMaxSpeed(MAX_SPEED);
            break;
        case 'F':
            cam->fl += 0.1;
            break;
        case 'G':
            cam->fl -= 0.1;
            break;
        case 'H':
            cam->fl = DEFAULT_FOCAL_LENGTH;
            break;
        default:
            break;
        }

        debug_obj(available());
        debug_log("End\n");
    }
}

bool Game::update()
{
    check_input();

    if (running)
        world.update();

    tick();

    return running;
}

void Game::render()
{
    renderer.clear();

    for (mesh m : world.render())
    {
        renderer.addMesh(m);
    }

    renderer.render();
}

void Game::run()
{
    renderer.init();

    last_tick = time_ms();

    cam = renderer.cam;

    running = true;

    while (update())
    {
        render();
    }

    renderer.quit();
}
