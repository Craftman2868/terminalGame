#include "3d/render.hpp"

#include <stdio.h>  // printf
#include <sys/ioctl.h>  // ioctl TIOCGWINSZ, winsize (get terminal width and height)
#include <stdint.h>  // uint8_t
#include <algorithm>

#include "debug.hpp"
#include "3d/model.hpp"

unsigned int w = 20, h = 20;
unsigned char *pixels;
camera cam;

#if TICKER
bool tick = true;
#endif

vec2 vecToScreen(vec2 v)
{
    return {(CHAR_RATIO * ((float) h) / ((float) w) * v.x + 1) * w / 2, (-v.y + 1) * h / 2};
}

triangle2 triangleToScreen(triangle2 tri)
{
    return {vecToScreen(tri.p1), vecToScreen(tri.p2), vecToScreen(tri.p3)};
}

void render_tick()
{
#if TICKER
    tick = !tick;
#endif
}

void draw()
{
    printf("\033[H");

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            putchar(LIGHT_GRADIENT[pixels[i * w + j] * (LIGHT_GRADIENT_LENGH-1) / 255]);
        }
    }

#if TICKER
    printf("\n%7.3f  %7.3f  %7.3f  /  %6.3f  %6.3f / %4.1f      %s", cam.pos.x, cam.pos.y, cam.pos.z, cam.pitch, cam.yaw, cam.fl, (tick ? "[o| ]" : "[ |o]"));
#else
    printf("\n%7.3f  %7.3f  %7.3f  /  %6.3f  %6.3f / %4.1f", cam.pos.x, cam.pos.y, cam.pos.z, cam.pitch, cam.yaw, cam.fl);
#endif
    fflush(stdout);
}

void fill(unsigned char px)
{
    for (unsigned int i = 0; i < w*h; i++)
        pixels[i] = px;
}

void clear_buffer()
{
    fill(0);
}

void putPixel(unsigned int x, unsigned int y, unsigned char px, unsigned char opacity)
{
    if (x < w && y < h)  // always >= 0 because unsigned
        pixels[y*w+x] = opacity * px / (255 - MIN_LIGHT) + MIN_LIGHT * 3 + pixels[y*w+x] * (255 - opacity) / (255 - MIN_LIGHT);
}

void putPixel(unsigned int x, unsigned int y, unsigned char px)
{
    if (x < w && y < h)  // always >= 0 because unsigned
        pixels[y*w+x] = px;
}

void putPixel(vec2 pos, unsigned char px, unsigned char opacity)
{
    putPixel(toInt(pos.x), toInt(pos.y), px, opacity);
}

void putPixel(vec2 pos, unsigned char px)
{
    putPixel(toInt(pos.x), toInt(pos.y), px);
}

void putScreenTriangle(triangle2 tri, unsigned char px, unsigned char opacity)
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
                    if (inTriangle(tri, {(float) x, (float) y}))
                        putPixel(x, y, px, opacity);
                }
}

void putScreenTriangle(triangle2 tri, unsigned char px)
{
    putScreenTriangle(tri, px, 255);
}

void putTriangle2(triangle2 tri, unsigned char px, unsigned char opacity)
{
    return putScreenTriangle(triangleToScreen(tri), px, opacity);
}

void putTriangle2(triangle2 tri, unsigned char px)
{
    return putScreenTriangle(triangleToScreen(tri), px);
}

void putTriangle3(triangle3 tri, unsigned char px, unsigned char opacity)
{
    return putTriangle2(projTriangle(cam, tri), px, opacity);
}

void putTriangle3(triangle3 tri, unsigned char px)
{
    return putTriangle2(projTriangle(cam, tri), px);
}

void putPoint(vec3 p, unsigned char px, unsigned char opacity)
{
    return putPixel(vecToScreen(projVec(cam, vec3Rotate(subVec3(p, cam.pos), cam.pitch, cam.yaw))), px, opacity);
}

void putPoint(vec3 p, unsigned char px)
{
    return putPoint(p, px, 255);
}

char diffuseLight(lightSource light, vec3 normal, vec3 vec)
{
    float intensity = dotVec3(normalizeVec3(subVec3(light.pos, vec)), normalizeVec3(normal));  // -1 < intensity < 1

    if (intensity <= 0)
        return 0;

    return intensity * 255;
}

uint8_t clipTriangle3(triangle3 tri, vec3 planeNormal, triangle3 *clippedTriangles)
{
    bool p1in, p2in, p3in;
    bool inverted;
    vec3 collision;
    vec3 zNear = addVec3(cam.pos, mulVec3(planeNormal, PLANE_DIST));

    p1in = dotVec3(subVec3(zNear, tri.p1), planeNormal) < 0;
    p2in = dotVec3(subVec3(zNear, tri.p2), planeNormal) < 0;
    p3in = dotVec3(subVec3(zNear, tri.p3), planeNormal) < 0;

    inverted = p1in && p3in;

    if (!p1in && !p2in && !p3in)  // No point in
    {
        return 0;
    }
    else if (p1in && p2in && p3in)  // Every point in
    {
        clippedTriangles[0] = tri;
        return 1;
    }
    else if (p1in ^ p2in ^ p3in)  // 1 point in
    {
        if (inverted)
            clippedTriangles[0] = {
                linePlaneCollision(planeNormal, zNear, (!p1in ? tri.p1 : tri.p2), (p1in ? tri.p1 : (p2in ? tri.p2 : tri.p3))),
                (p1in ? tri.p1 : (p2in ? tri.p2 : tri.p3)),
                linePlaneCollision(planeNormal, zNear, (!p3in ? tri.p3 : tri.p2), (p1in ? tri.p1 : (p2in ? tri.p2 : tri.p3))),
            };
        else
            clippedTriangles[0] = {
                linePlaneCollision(planeNormal, zNear, (!p1in ? tri.p1 : tri.p2), (p1in ? tri.p1 : (p2in ? tri.p2 : tri.p3))),
                linePlaneCollision(planeNormal, zNear, (!p3in ? tri.p3 : tri.p2), (p1in ? tri.p1 : (p2in ? tri.p2 : tri.p3))),
                (p1in ? tri.p1 : (p2in ? tri.p2 : tri.p3)),
            };

        return 1;
    }
    else  // 2 points in
    {
        if (inverted)
        {
            collision = linePlaneCollision(planeNormal, zNear, (!p1in ? tri.p1 : (!p2in ? tri.p2 : tri.p3)), (p1in ? tri.p1 : tri.p2));
            clippedTriangles[0] = {
                linePlaneCollision(planeNormal, zNear, (!p1in ? tri.p1 : (!p2in ? tri.p2 : tri.p3)), (p3in ? tri.p3 : tri.p2)),
                (p3in ? tri.p3 : tri.p2),
                collision,
            };
            clippedTriangles[1] = {
                collision,
                (p3in ? tri.p3 : tri.p2),
                (p1in ? tri.p1 : tri.p2),
            };
        }
        else
        {
            collision = linePlaneCollision(planeNormal, zNear, (!p1in ? tri.p1 : (!p2in ? tri.p2 : tri.p3)), (p3in ? tri.p3 : tri.p2));
            clippedTriangles[0] = {
                linePlaneCollision(planeNormal, zNear, (!p1in ? tri.p1 : (!p2in ? tri.p2 : tri.p3)), (p1in ? tri.p1 : tri.p2)),
                (p1in ? tri.p1 : tri.p2),
                collision,
            };
            clippedTriangles[1] = {
                collision,
                (p1in ? tri.p1 : tri.p2),
                (p3in ? tri.p3 : tri.p2),
            };
        }

        return 2;
    }
}

float triangleDist(triangle3 tri)
{
    vec3 pos = mulVec3(addVec3(tri.p1, addVec3(tri.p2, tri.p3)), (float) 1/3);

    subVec3(&pos, cam.pos);

    return lengthVec3(pos);
}

bool compTriangleDist(renderTriangle tri1, renderTriangle tri2)
{
    return triangleDist(tri1.tri) > triangleDist(tri2.tri);
}

bool isVisible(triangle3 tri)
{
    vec3 normal = mulVec3(triangleNormal(tri), -1);

    return dotVec3(normal, subVec3(tri.p1, cam.pos)) > 0;
}

void putMesh(mesh m, lightSource light)
{
    triangle3 clippedTriangles[2];
    uint8_t triN = 0;
    char ch;

    std::sort(m.begin(), m.end(), compTriangleDist);

    for (renderTriangle rt : m)
    {
        triN = clipTriangle3(rt.tri, CAM_LOOK_AT_DIRECTION(cam), clippedTriangles);

        for (uint8_t i = 0; i < triN; i++)
            if (isVisible(clippedTriangles[i]))
            {
                ch = diffuseLight(light, triangleNormal(clippedTriangles[i]), clippedTriangles[i].p1);
                putTriangle3(triangle3Rotate(triangle3Translate(clippedTriangles[i], mulVec3(cam.pos, -1)), cam.pitch, cam.yaw), ch, rt.opacity);
            }
    }
}

void putMesh(mesh m)
{
    
    triangle3 clippedTriangles[2];
    uint8_t triN = 0;

    std::sort(m.begin(), m.end(), compTriangleDist);

    for (renderTriangle rt : m)
    {
        triN = clipTriangle3(rt.tri, CAM_LOOK_AT_DIRECTION(cam), clippedTriangles);

        for (uint8_t i = 0; i < triN; i++)
            if (isVisible(clippedTriangles[i]))
            {
                putTriangle3(triangle3Rotate(triangle3Translate(clippedTriangles[i], mulVec3(cam.pos, -1)), cam.pitch, cam.yaw), MIN_LIGHT, rt.opacity);
            }
    }
}

camera *render_init()
{
    struct winsize ws;
    ioctl(0, TIOCGWINSZ, &ws);

    w = ws.ws_col;
    h = ws.ws_row - 1;

    cam = {{0, 0, 0}, 0, 0, DEFAULT_FOCAL_LENGTH};

    pixels = new unsigned char[w * h + 1];
    pixels[w * h] = 0;
    clear_buffer();

    printf("\033[?1049h\033[?25l\033[2J");
    draw();

    return &cam;
}

void render_quit()
{
    printf("\033[H\033[?1049l\033[?25h");
    fflush(stdout);

    delete[] pixels;
}
