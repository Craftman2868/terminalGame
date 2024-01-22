#pragma once

#include <vector>
#include <stdint.h>  // uint8_t

#include "maths.hpp"

#define STDOUT 1
#define CHAR_RATIO 2 / 1  // char height / char width

#define MESH_CHAR 'O'

#define PLANE_DIST 0.001

typedef std::vector<triangle3> mesh;

vec2 vecToScreen(vec2 v);
triangle2 triangleToScreen(triangle2 tri);

void draw();
void fill(char px);
void clear();

void putPixel(unsigned int x, unsigned int y, char px);
void putPixel(vec2 pos, char px);

double eq(vec2 p, vec2 a, vec2 b);
void putScreenTriangle(triangle2 tri, char px);
void putTriangle2(triangle2 tri, char px);
void putTriangle3(triangle3 tri, char px);

uint8_t clipTriangle3(triangle3 tri, vec3 planeNormal, triangle3 *clippedTriangles);

void putMesh(mesh m);

camera *init();
void quit();
