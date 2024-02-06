#ifndef RENDER_QUADTREE_H
#define RENDER_QUADTREE_H

#include "QuadTree.h"

//#include "GL/GL.h"
//#include "GL/GLU.h"
//#include "GL/glext.h"
//#include "GL/glcorearb.h"
//#include "GL/glxext.h"
//#include "GL/wglext.h"

#include <GL\glew.h>
//#include "GL\glfw3.h"
////#include "GL\glfw3native.h"

//#include <glad/gl.h>
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>

class Render_QuadTree
{
public:
	QuadTree* scene;

	void init(QuadTree* _scene) 
	{
		scene = _scene;

	}

	void render(Matrix4* camera, Vector3f* camera_pos, Quaternion* camera_orientation, float camera_angle_pitch, float  camera_angle_yaw, float camera_angle_roll, Vector3f** camera_front, Vector3f** camera_up)
	{
		auto scene_bounds = scene->boundary;
		auto range = scene->query_render_range(scene_bounds, camera, camera_pos, camera_orientation, camera_angle_pitch, camera_angle_yaw, camera_angle_roll, camera_front, camera_up);
		for (int i = 0; i < range->size(); i++)
		{
			auto scene_element = range->at(i);

			scene_element->render(camera, camera_pos, camera_orientation, camera_angle_pitch, camera_angle_yaw, camera_angle_roll, camera_front, camera_up);
		}
		scene_bounds->render(camera, camera_pos, camera_orientation, camera_angle_pitch, camera_angle_yaw, camera_angle_roll, camera_front, camera_up);
	}
};

#endif