#include "3d/model.hpp"

#include <stdio.h>
#include <vector>
#include <stdexcept>

#include "3d/maths.hpp"

mesh loadModel(char *path)
{
    mesh m;
    vec3 v;
    int vertexIds[4];
    std::vector<vec3> vertex;
    unsigned int opacity = DEFAULT_OPACITY;

    int ch = 0;

    FILE *f = fopen(path, "r");

    if (f == nullptr)
        return m;

    while (ch != EOF)
    {
        ch = fgetc(f);

        if (ch == EOF)
            break;

        if (ch == 'v' && fgetc(f) == ' ')
        {
            if (fscanf(f, "%f %f %f", &v.x, &v.y, &v.z) != 3)
                throw std::runtime_error("Invalid vertex definition in " + std::string(path));

            vertex.push_back(v);
        }
        else if (ch == 'f' && fgetc(f) == ' ')
        {
            if (fscanf(f, " %d %d %d", &vertexIds[0], &vertexIds[1], &vertexIds[2]) != 3)
                throw std::runtime_error("Invalid face definition in " + std::string(path));
            
            ch = fgetc(f);

            if (ch == ' ')
            {
                if (fscanf(f, "%d", &vertexIds[3]) != 1)
                    throw std::runtime_error("Invalid face definition in " + std::string(path));
            }
            else
            {
                vertexIds[3] = -1;
            }

            for (int i = 0; i < 4; i++)
            {
                if (vertexIds[i] != -1 && vertexIds[i] > vertex.size())
                    throw std::runtime_error("Invalid face definition in " + std::string(path) + ": Vertex " + std::to_string(i) + " not found");
            }

            m.push_back({{vertex[vertexIds[0]-1], vertex[vertexIds[1]-1], vertex[vertexIds[2]-1]}, (unsigned char) opacity});

            if (vertexIds[3] != -1)
            {
                m.push_back({{vertex[vertexIds[2]-1], vertex[vertexIds[3]-1], vertex[vertexIds[0]-1]}, (unsigned char) opacity});
            }
        }
        else if (ch == '#' && fscanf(f, "opacity %u", &opacity) == 1)
        {
            // opacity changed
        }

        while (ch != '\n' && ch != '\r')
        {
            ch = fgetc(f);

            if (ch == EOF)
                break;
        }
    }

    fclose(f);

    return m;
}

void moveMesh(mesh *m, vec3 v)
{
    for (int i = 0; i < m->size(); i++)
    {
        triangle3Translate(&(*m)[i].tri, v);
    }
}

mesh moveMesh(mesh m, vec3 v)
{
    mesh new_m;

    for (int i = 0; i < m.size(); i++)
    {
        new_m.push_back({triangle3Translate(m[i].tri, v), m[i].opacity});
    }

    return new_m;
}
