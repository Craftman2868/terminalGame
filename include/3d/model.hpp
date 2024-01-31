#pragma once

#include <vector>

#include "3d/maths.hpp"

#define DEFAULT_OPACITY 255  // 0 to 255

typedef struct {
    triangle3 tri;
    unsigned char opacity;
} renderTriangle;

typedef std::vector<renderTriangle> mesh;

mesh loadModel(char *path);

void moveMesh(mesh *m, vec3 v);
mesh moveMesh(mesh m, vec3 v);
