#include "QueryRenderSample.h"

void QueryRenderSample::render(Matrix4* camera, Vector3f* camera_pos, Quaternion* camera_orientation, float camera_angle_pitch, float camera_angle_yaw, float camera_angle_roll, Vector3f** camera_front, Vector3f** camera_up)
{
    shader_program->use(camera, camera_pos, camera_orientation, camera_angle_pitch, camera_angle_yaw, camera_angle_roll, camera_front, camera_up);
}