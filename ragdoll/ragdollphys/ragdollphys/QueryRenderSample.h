#ifndef QUERYRENDERSAMPLE_H
#define QUERYRENDERSAMPLE_H

#include <vector>

#include "Vector2f.h"
#include "Vector3f.h"
#include "Matrix4.h"
#include "ShaderProgram.h"

class QueryRenderSample {
public:
    std::vector<Vector2f*>* sample_points;
    ShaderProgram* shader_program;
    ShaderProgram* shader_program_vertex;
    ShaderProgram* shader_program_fragment;
    bool initilised = false;
    void render(Matrix4* camera, Vector3f* camera_pos, Quaternion* camera_orientation, float cameraAngle);
};

#endif