#include "renderer.hpp"

#include <stdexcept>

#include "3d/render.hpp"
#include "input.hpp"

Renderer::Renderer(lightSource light)
{
    this->light = light;
    this->cam = nullptr;
}

void Renderer::init()
{
    if (cam != nullptr)
        return;

    input_init(false);

    cam = render_init();
}

void Renderer::quit()
{
    render_quit();

    input_quit();

    cam = nullptr;
}

void Renderer::clear()
{
    triangles.clear();
}

void Renderer::addMesh(mesh m)
{
    for (renderTriangle rt : m)
    {
        triangles.push_back(rt);
    }
}

void Renderer::render()
{
    if (cam == nullptr)
        throw std::runtime_error("Renderer not initialized");

    clear_buffer();
    putMesh(triangles);
    draw();
}

Renderer::~Renderer()
{
    if (cam != nullptr)
        quit();
}
