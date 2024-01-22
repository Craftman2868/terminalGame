#pragma once

#include <cmath>

// #include <vector>

#define toInt(n) ((int) std::round(n))

// 2D

typedef struct
{
    double x;
    double y;
} vec2;

typedef struct
{
    vec2 p1;
    vec2 p2;
    vec2 p3;
} triangle2;

vec2 addVec2(vec2 v1, vec2 v2);
vec2 subVec2(vec2 v1, vec2 v2);
vec2 mulVec2(vec2 v1, vec2 v2);
vec2 mulVec2(vec2 v, double n);

// 3D

typedef struct
{
    double x;
    double y;
    double z;
} vec3;

typedef struct
{
    vec3 pos;
    double pitch, yaw;
    float fl;  // focal length
} camera;

#define CAM_FORWARD_DIRECTION(cam)    vec3 {-std::sin((cam).yaw), 0, std::cos((cam).yaw)}
#define CAM_RIGHT_DIRECTION(cam)      vec3 { std::cos((cam).yaw), 0, std::sin((cam).yaw)}
#define CAM_LOOK_AT_DIRECTION(cam)    vec3 {-std::sin((cam).yaw) * std::cos((cam).pitch), std::sin((cam).pitch), std::cos((cam).yaw) * std::cos((cam).pitch)}

typedef struct
{
    vec3 p1;
    vec3 p2;
    vec3 p3;
} triangle3;

vec3 addVec3(vec3 v1, vec3 v2);
vec3 *addVec3(vec3 *v1, vec3 v2);
vec3 subVec3(vec3 v1, vec3 v2);
vec3 *subVec3(vec3 *v1, vec3 v2);
vec3 mulVec3(vec3 v1, vec3 v2);
vec3 *subVec3(vec3 *v1, vec3 v2);
vec3 mulVec3(vec3 v, double n);
vec3 *mulVec3(vec3 *v1, double n);

double dotVec3(vec3 v1, vec3 v2);
vec3 crossProdVec3(vec3 v1, vec3 v2);

vec2 projVec(camera cam, vec3 p);
triangle2 projTriangle(camera cam, triangle3 tri3);

triangle3 triangle3Translate(triangle3 tri, vec3 vec);
triangle3* triangle3Translate(triangle3 *tri, vec3 vec);

vec3 vec3RotateX(vec3 vec, double pitch);
vec3 vec3RotateY(vec3 vec, double yaw);

triangle3 triangle3RotateX(triangle3 tri, double pitch);
triangle3 *triangle3RotateX(triangle3 *tri, double pitch);
triangle3 triangle3RotateY(triangle3 tri, double yaw);
triangle3 *triangle3RotateY(triangle3 *tri, double yaw);

triangle3 triangle3Rotate(triangle3 tri, double pitch, double yaw);
triangle3 *triangle3Rotate(triangle3 *tri, double pitch, double yaw);

vec3 linePlaneCollision(vec3 planeNormal, vec3 planePoint, vec3 v1, vec3 v2);
