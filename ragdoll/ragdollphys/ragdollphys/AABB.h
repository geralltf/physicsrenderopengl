#ifndef AABB_H
#define AABB_H

#include "VectorLib.h"
#include "Vector2f.h"
#include "ShaderProgram.h"

// Axis-aligned bounding box with half dimension and center
class AABB
{
public:
    Vector2f* center = nullptr;
    float half_dimension;
    ShaderProgram* shader_program;
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    bool has_init = false;
    AABB();
    AABB(const Vector2f* _center, const float _half_dimension);
    bool contains_point(const Vector2f* point);
    bool intersects_AABB(const AABB* other);
    void render(Matrix4* camera, Vector3f* camera_pos, Quaternion* camera_orientation, float camera_angle_pitch, float camera_angle_yaw, float camera_angle_roll, Vector3f** camera_front, Vector3f** camera_up, bool show_wireframe);
};

#endif