#include "3d/maths.hpp"

#include <cmath>

// 2D

vec2 addVec2(vec2 v1, vec2 v2)
{
    return {v1.x + v2.x, v1.y + v2.y};
}

vec2 subVec2(vec2 v1, vec2 v2)
{
    return {v1.x - v2.x, v1.y - v2.y};
}

vec2 mulVec2(vec2 v1, vec2 v2)
{
    return {v1.x * v2.x, v1.y * v2.y};
}

vec2 mulVec2(vec2 v, double n)
{
    return {v.x * n, v.y * n};
}

// 3D

vec3 addVec3(vec3 v1, vec3 v2)
{
    return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

vec3 *addVec3(vec3 *v1, vec3 v2)
{
    v1->x += v2.x;
    v1->y += v2.y;
    v1->z += v2.z;

    return v1;
}

vec3 subVec3(vec3 v1, vec3 v2)
{
    return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

vec3 *subVec3(vec3 *v1, vec3 v2)
{
    v1->x -= v2.x;
    v1->y -= v2.y;
    v1->z -= v2.z;

    return v1;
}

vec3 mulVec3(vec3 v1, vec3 v2)
{
    return {v1.x * v2.x, v1.y * v2.y, v1.z * v2.z};
}

vec3 *mulVec3(vec3 *v1, vec3 v2)
{
    v1->x *= v2.x;
    v1->y *= v2.y;
    v1->z *= v2.z;

    return v1;
}

vec3 mulVec3(vec3 v, double n)
{
    return {v.x * n, v.y * n, v.z * n};
}

vec3 *mulVec3(vec3 *v1, double n)
{
    v1->x *= n;
    v1->y *= n;
    v1->z *= n;

    return v1;
}

double dotVec3(vec3 v1, vec3 v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vec3 crossProdVec3(vec3 v1, vec3 v2)
{
    return {v1.y*v2.z-v1.z*v2.y,
            v1.z*v2.x-v1.x*v2.z, 
            v1.x*v2.y-v1.y*v2.x};
}

double lengthVec3(vec3 vec)
{
    return std::sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

vec3 normalizeVec3(vec3 vec)
{
    double norm = lengthVec3(vec);
    return {vec.x/norm, vec.y/norm, vec.z/norm};
}

vec3 *normalizeVec3(vec3 *vec)
{
    double norm = std::sqrt(vec->x*vec->x + vec->y*vec->y + vec->z*vec->z);

    vec->x /= norm;
    vec->y /= norm;
    vec->z /= norm;

    return vec;
}

vec2 projVec(camera cam, vec3 p)
{
    return {p.x*cam.fl/p.z, p.y*cam.fl/p.z};
}

triangle2 projTriangle(camera cam, triangle3 tri)
{
    return {
        projVec(cam, tri.p1),
        projVec(cam, tri.p2),
        projVec(cam, tri.p3),
    };
}

triangle3 triangle3Translate(triangle3 tri, vec3 vec)
{
    return {
        addVec3(tri.p1, vec),
        addVec3(tri.p2, vec),
        addVec3(tri.p3, vec)
    };
}

triangle3 *triangle3Translate(triangle3 *tri, vec3 vec)
{
    tri->p1 = addVec3(tri->p1, vec);
    tri->p2 = addVec3(tri->p2, vec);
    tri->p3 = addVec3(tri->p3, vec);

    return tri;
}

vec3 vec3RotateX(vec3 v, double pitch)
{
    return {
        v.x,
        std::cos(pitch)*v.y-std::sin(pitch)*v.z,
        std::sin(pitch)*v.y+std::cos(pitch)*v.z,
    };
}

vec3 vec3RotateY(vec3 v, double yaw)
{
    return {
        std::cos(yaw)*v.x+std::sin(yaw)*v.z,
        v.y,
        -std::sin(yaw)*v.x+std::cos(yaw)*v.z,
    };
}

vec3 vec3Rotate(vec3 v, double pitch, double yaw)
{
    return vec3RotateX(vec3RotateY(v, yaw), pitch);
}

triangle3 triangle3RotateX(triangle3 tri, double pitch)
{
    return {
        vec3RotateX(tri.p1, pitch),
        vec3RotateX(tri.p2, pitch),
        vec3RotateX(tri.p3, pitch)
    };
}

triangle3 *triangle3RotateX(triangle3 *tri, double pitch)
{
    tri->p1 = vec3RotateX(tri->p1, pitch);
    tri->p2 = vec3RotateX(tri->p2, pitch);
    tri->p3 = vec3RotateX(tri->p3, pitch);

    return tri;
}

triangle3 triangle3RotateY(triangle3 tri, double yaw)
{
    return {
        vec3RotateY(tri.p1, yaw),
        vec3RotateY(tri.p2, yaw),
        vec3RotateY(tri.p3, yaw)
    };
}

triangle3 *triangle3RotateY(triangle3 *tri, double yaw)
{
    tri->p1 = vec3RotateY(tri->p1, yaw);
    tri->p2 = vec3RotateY(tri->p2, yaw);
    tri->p3 = vec3RotateY(tri->p3, yaw);

    return tri;
}

triangle3 triangle3Rotate(triangle3 tri, double pitch, double yaw)
{
    return triangle3RotateX(triangle3RotateY(tri, yaw), pitch);
}

triangle3 *triangle3Rotate(triangle3 *tri, double pitch, double yaw)
{
    triangle3RotateY(tri, yaw);
    triangle3RotateX(tri, pitch);

    return tri;
}

vec3 linePlaneCollision(vec3 planeNormal, vec3 planePoint, vec3 v1, vec3 v2)
{
    vec3 u = subVec3(v2, v1);
    double dotp = dotVec3(planeNormal, u);
    
    if (std::abs(dotp) < 1e-5)
        return {0, 0, 0};  // No collision

    return addVec3(v1, mulVec3(u, -dotVec3(planeNormal, subVec3(v1, planePoint)) / dotp));
}
