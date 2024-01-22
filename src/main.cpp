#include <unistd.h>  // usleep
#include <stdio.h>

#include "debug.hpp"

#include "maths.hpp"
#include "render.hpp"
#include "input.hpp"

#define SPEED 0.03

bool running = false;

int main(int argc, char *argv[])
{
    Key key;
    bool updated;
    mesh m;

    debug_init();

    input_init();
    camera *cam = init();

    // Cude:  // TODO

    m.push_back({{0, 1, 1}, {0, 1, 2}, {1, 1, 1}}); // Top
    m.push_back({{1, 1, 2}, {0, 1, 2}, {1, 1, 1}}); // Top

    m.push_back({{0, 1, 1}, {0, 0, 1}, {1, 1, 1}}); // Front
    m.push_back({{1, 0, 1}, {0, 0, 1}, {1, 1, 1}}); // Front

    m.push_back({{1, 0, 2}, {1, 0, 1}, {1, 1, 1}}); // Right
    m.push_back({{1, 1, 2}, {1, 1, 1}, {1, 0, 2}}); // Right

    m.push_back({{0, 0, 1}, {0, 0, 2}, {1, 0, 1}}); // Bottom
    m.push_back({{1, 0, 2}, {0, 0, 2}, {1, 0, 1}}); // Bottom

    m.push_back({{0, 1, 2}, {0, 0, 2}, {1, 1, 2}}); // Back
    m.push_back({{1, 0, 2}, {0, 0, 2}, {1, 1, 2}}); // Back

    m.push_back({{0, 0, 2}, {0, 0, 1}, {0, 1, 1}}); // Left
    m.push_back({{0, 1, 2}, {0, 1, 1}, {0, 0, 2}}); // Left
    
    // // Square

    // m.push_back({{0, 1, 2}, {0, 0, 2}, {1, 1, 2}});
    // m.push_back({{1, 0, 2}, {0, 0, 2}, {1, 1, 2}});

    debug_log("Running...\n");

    running = true;
    while (running)
    {
        debug_log("-------------------------\n");
        clear();

        putMesh(m);

        draw();

        updated = false;
        while (!updated || hasch())
        {
            getkey(&key);

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
                    if (cam->pitch > -1.57)
                        cam->pitch -= SPEED;
                    updated = true;
                    break;
                case KEY_UP:
                    if (cam->pitch < 1.57)
                        cam->pitch += SPEED;
                    updated = true;
                    break;
                case KEY_LEFT:
                    cam->yaw += SPEED;
                    updated = true;
                    break;
                case KEY_RIGHT:
                    cam->yaw -= SPEED;
                    updated = true;
                    break;
                default:
                    break;
                }
                break;
            case 'Z':
                addVec3(&cam->pos, mulVec3(CAM_FORWARD_DIRECTION(*cam), SPEED));
                updated = true;
                break;
            case 'S':
                subVec3(&cam->pos, mulVec3(CAM_FORWARD_DIRECTION(*cam), SPEED));
                updated = true;
                break;
            case 'Q':
                subVec3(&cam->pos, mulVec3(CAM_RIGHT_DIRECTION(*cam), SPEED));
                updated = true;
                break;
            case 'D':
                addVec3(&cam->pos, mulVec3(CAM_RIGHT_DIRECTION(*cam), SPEED));
                updated = true;
                break;
            case 'E':
            case ' ':
                cam->pos.y += SPEED;
                updated = true;
                break;
            case 'A':
                cam->pos.y -= SPEED;
                updated = true;
                break;
            case '0':
                cam->pos = {0, 0, 0};
                cam->pitch = 0;
                cam->yaw = 0;
                updated = true;
                break;
            default:
                break;
            }
        }
    }

    quit();
    input_quit();

    debug_log("Quit\n");

    debug_quit();

    return 0;
}
