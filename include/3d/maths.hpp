#pragma once

#include <cmath>

// #include <vector>

#define toInt(n) ((int) std::round(n))

// 2D

typedef struct
{
    float x;
    float y;
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
vec2 mulVec2(vec2 v, float n);

// 3D

typedef struct vec3
{
    float x;
    float y;
    float z;
} vec3;

typedef struct
{
    vec3 pos;
    float pitch, yaw;
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
vec3 mulVec3(vec3 v, float n);
vec3 *mulVec3(vec3 *v1, float n);

float dotVec3(vec3 v1, vec3 v2);
vec3 crossProdVec3(vec3 v1, vec3 v2);

float lengthVec3(vec3 vec);

vec3 normalizeVec3(vec3 vec);
vec3 *normalizeVec3(vec3 *vec);

vec2 projVec(camera cam, vec3 p);
triangle2 projTriangle(camera cam, triangle3 tri3);

triangle3 triangle3Translate(triangle3 tri, vec3 vec);
triangle3* triangle3Translate(triangle3 *tri, vec3 vec);

vec3 vec3RotateX(vec3 vec, float pitch);
vec3 vec3RotateY(vec3 vec, float yaw);
vec3 vec3Rotate(vec3 vec, float pitch, float yaw);

triangle3 triangle3RotateX(triangle3 tri, float pitch);
triangle3 *triangle3RotateX(triangle3 *tri, float pitch);
triangle3 triangle3RotateY(triangle3 tri, float yaw);
triangle3 *triangle3RotateY(triangle3 *tri, float yaw);

triangle3 triangle3Rotate(triangle3 tri, float pitch, float yaw);
triangle3 *triangle3Rotate(triangle3 *tri, float pitch, float yaw);

vec3 triangleNormal(triangle3 tri);

vec3 linePlaneCollision(vec3 planeNormal, vec3 planePoint, vec3 v1, vec3 v2);

bool vec3eq(vec3 v1, vec3 v2);

bool inTriangle(triangle2 tri, vec2 p);
