#include <unistd.h>  // usleep
#include <stdio.h>

#include "debug.hpp"

#include "maths.hpp"
#include "render.hpp"
#include "input.hpp"
#include "model.hpp"

#define DEFAULT_SPEED 0.03

bool running = false;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s file.obj\n", argv[0]);

        return 1;
    }

    float speed = DEFAULT_SPEED;
    Key key;
    bool updated;
    mesh m;
    lightSource light = {{-1, 2, 0}};

    debug_init();

    m = loadModel(argv[1]);

    // moveMesh(&m, {0, 0, 8});

    if (m.size() == 0)
    {
        debug_quit();
        return 1;
    }

    debug_obj(m);

    input_init();
    camera *cam = init();

    cam->pos = {0, 1, -3};

    debug_log("Running...\n");

    running = true;
    while (running)
    {
        debug_log("-------------------------\n");
        clear();

        putMesh(m, light);
        // putPoint(light.pos, 'L');

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
                        cam->pitch -= speed;
                    updated = true;
                    break;
                case KEY_UP:
                    if (cam->pitch < 1.57)
                        cam->pitch += speed;
                    updated = true;
                    break;
                case KEY_LEFT:
                    cam->yaw += speed;
                    updated = true;
                    break;
                case KEY_RIGHT:
                    cam->yaw -= speed;
                    updated = true;
                    break;
                default:
                    break;
                }
                break;
            case 'Z':
                addVec3(&cam->pos, mulVec3(CAM_FORWARD_DIRECTION(*cam), speed));
                updated = true;
                break;
            case 'S':
                subVec3(&cam->pos, mulVec3(CAM_FORWARD_DIRECTION(*cam), speed));
                updated = true;
                break;
            case 'Q':
                subVec3(&cam->pos, mulVec3(CAM_RIGHT_DIRECTION(*cam), speed));
                updated = true;
                break;
            case 'D':
                addVec3(&cam->pos, mulVec3(CAM_RIGHT_DIRECTION(*cam), speed));
                updated = true;
                break;
            case 'E':
            case ' ':
                cam->pos.y += speed;
                updated = true;
                break;
            case 'A':
                cam->pos.y -= speed;
                updated = true;
                break;
            case '0':
                cam->pos = {0, 0, 0};
                cam->pitch = 0;
                cam->yaw = 0;
                speed = DEFAULT_SPEED;
                updated = true;
                break;
            case '+':
                if (speed < 0.001)
                    speed = 0.005;
                else if (speed > 10)
                    break;
                speed *= 2;
                break;
            case '-':
                speed /= 2;
                break;
            case '*':
                speed = DEFAULT_SPEED;
                break;
            case 'F':
                cam->fl += 0.1;
                updated = true;
                break;
            case 'G':
                cam->fl -= 0.1;
                updated = true;
                break;
            case 'H':
                cam->fl = DEFAULT_FOCAL_LENGTH;
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
