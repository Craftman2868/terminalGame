#include "model.hpp"

#include <stdio.h>
#include <vector>
#include <stdexcept>

#include "maths.hpp"

mesh loadModel(char *path)
{
    mesh m;
    vec3 v;
    int vertexIds[4];
    std::vector<vec3> vertex;

    char ch = 0;

    FILE *f = fopen(path, "r");

    if (f == nullptr)
        return mesh();

    while (ch != EOF)
    {
        ch = fgetc(f);

        if (ch == EOF)
            break;
        
        if (ch == 'v' && fgetc(f) == ' ')
        {
            if (fscanf(f, "%lf %lf %lf", &v.x, &v.y, &v.z) != 3)
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

            m.push_back({{vertex[vertexIds[0]-1], vertex[vertexIds[1]-1], vertex[vertexIds[2]-1]}, DEFAULT_OPACITY});

            if (vertexIds[3] != -1)
            {
                m.push_back({{vertex[vertexIds[2]-1], vertex[vertexIds[3]-1], vertex[vertexIds[0]-1]}, DEFAULT_OPACITY});
            }
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
