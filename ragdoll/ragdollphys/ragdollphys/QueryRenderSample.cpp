#include "QueryRenderSample.h"

void QueryRenderSample::render(Matrix4* camera, Vector3f* camera_pos, Quaternion* camera_orientation, float cameraAngle)
{
    shader_program->use(camera, camera_pos, camera_orientation, cameraAngle);
}