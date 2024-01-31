#pragma once

#include <vector>
#include <stdint.h>  // uint8_t

#include "3d/maths.hpp"
#include "3d/model.hpp"

#define STDOUT 1
#define CHAR_RATIO 2 / 1  // char height / char width

#define DEFAULT_FOCAL_LENGTH 2

#define PLANE_DIST 0.001

#define LIGHT_GRADIENT " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@"
#define LIGHT_GRADIENT_LENGH (sizeof (LIGHT_GRADIENT) - 1)
#define MIN_LIGHT (255 / LIGHT_GRADIENT_LENGH + 255 % LIGHT_GRADIENT_LENGH ? 1 : 0)

typedef struct {
    vec3 pos;
} lightSource;

vec2 vecToScreen(vec2 v);
triangle2 triangleToScreen(triangle2 tri);

void draw();
void fill(unsigned char px);
void clear_buffer();

void putPixel(unsigned int x, unsigned int y, unsigned char px, unsigned char opacity);
void putPixel(unsigned int x, unsigned int y, unsigned char px);
void putPixel(vec2 pos, unsigned char px, unsigned char opacity);
void putPixel(vec2 pos, unsigned char px);

double eq(vec2 p, vec2 a, vec2 b);
void putScreenTriangle(triangle2 tri, unsigned char px, unsigned char opacity);
void putScreenTriangle(triangle2 tri, unsigned char px);
void putTriangle2(triangle2 tri, unsigned char px, unsigned char opacity);
void putTriangle2(triangle2 tri, unsigned char px);
void putTriangle3(triangle3 tri, unsigned char px, unsigned char opacity);
void putTriangle3(triangle3 tri, unsigned char px);

void putPoint(vec3 p, unsigned char px, unsigned char opacity);
void putPoint(vec3 p, unsigned char px);

char diffuseLight(lightSource light, vec3 normal, vec3 vec);

uint8_t clipTriangle3(triangle3 tri, vec3 planeNormal, triangle3 *clippedTriangles);

double triangleDist(triangle3 tri);  // The distance between the triangle and the camera

void putMesh(mesh m);
void putMesh(mesh m, lightSource light);

camera *render_init();
void render_quit();
