#pragma once

#include "3d/maths.hpp"
#include "3d/model.hpp"
#include "3d/render.hpp"

class Renderer
{
private:
    mesh triangles;
    lightSource light;

public:
    camera *cam;

    Renderer(lightSource light);

    void init();
    void quit();

    lightSource getLight() { return light; }
    void setLight(lightSource light) { this->light = light; }

    void clear();
    void addMesh(mesh m);
    void render();

    ~Renderer();
};
