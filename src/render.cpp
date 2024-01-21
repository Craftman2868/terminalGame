#include <stdio.h>  // printf
#include <sys/ioctl.h>  // ioctl TIOCGWINSZ, winsize (get terminal width and height)

#include "render.hpp"

int w = 20, h = 20;
char *pixels;
camera cam;


vec2 vecToScreen(vec2 v)
{
    return {(CHAR_RATIO * ((double) h) / ((double) w) * v.x + 1) * w / 2, (-v.y + 1) * h / 2};
}

triangle2 triangleToScreen(triangle2 tri)
{
    return {vecToScreen(tri.p1), vecToScreen(tri.p2), vecToScreen(tri.p3)};
}

void draw()
{
    printf("\033[H%s\n", pixels);
    printf("%7.3f  %7.3f  %7.3f  /  %6.3f  %6.3f", cam.pos.x, cam.pos.y, cam.pos.z, cam.pitch, cam.yaw);
    fflush(stdout);
}

void fill(char px)
{
    for (int i = 0; i < w*h; i++)
        pixels[i] = px;
}

void clear()
{
    fill(' ');
}

void putPixel(unsigned int x, unsigned int y, char px)
{
    pixels[y*w+x] = px;
}

void putPixel(vec2 pos, char px)
{
    putPixel(toInt(pos.x), toInt(pos.y), px);
}

double eq(vec2 p, vec2 a, vec2 b)
{
    return (a.x-p.x)*(b.y-p.y)-(a.y-p.y)*(b.x-p.x);
}

void putScreenTriangle(triangle2 tri, char px)
{
    int xmin = toInt(std::min(std::min(tri.p1.x, tri.p2.x), tri.p3.x));
    int xmax = toInt(std::max(std::max(tri.p1.x, tri.p2.x), tri.p3.x));
    int ymin = toInt(std::min(std::min(tri.p1.y, tri.p2.y), tri.p3.y));
    int ymax = toInt(std::max(std::max(tri.p1.y, tri.p2.y), tri.p3.y));

    for (int y = ymin; y < ymax; y++)
        if (0 <= y && y < h)
            for (int x = xmin; x < xmax; x++)
                if (0 <= x && x < w)
                {
                    vec2 pos = {(double) x, (double) y};
                    double w1 = eq(pos, tri.p3, tri.p1);
                    double w2 = eq(pos, tri.p1, tri.p2);
                    double w3 = eq(pos, tri.p2, tri.p3);

                    if ((w1 >= 0 && w2 >= 0 && w3 >= 0) || (-w1 >= 0 && -w2 >= 0 && -w3 >= 0))
                        putPixel(pos, px);
                }
}

void putTriangle2(triangle2 tri, char px)
{
    return putScreenTriangle(triangleToScreen(tri), px);
}

void putTriangle3(triangle3 tri, char px)
{
    return putTriangle2(projTriangle(cam, tri), px);
}

void putMesh(mesh m)
{
    for (triangle3 tri : m)
    {
        putTriangle3(triangle3Rotate(triangle3Translate(tri, mulVec3(cam.pos, -1)), cam.pitch, cam.yaw), MESH_CHAR);
    }
}

camera *init()
{
    struct winsize ws;
    ioctl(0, TIOCGWINSZ, &ws);

    w = ws.ws_col;
    h = ws.ws_row - 1;

    cam = {{0, 0, 0}, 0, 0, 1};

    pixels = new char[w * h + 1];
    pixels[w * h] = 0;
    clear();

    printf("\033[?1049h\033[?25l\033[2J");
    draw();

    return &cam;
}

void quit()
{
    printf("\033[H\033[?1049l\033[?25h");
    fflush(stdout);

    delete[] pixels;
}
