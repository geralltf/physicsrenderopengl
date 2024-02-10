#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include <vector>
#include <iostream>
#include <string>

#include "AABB.h"
#include "VectorLib.h"
#include "Vector2f.h"
#include "Matrix4.h"
#include "QuadTree.h"
#include "RenderQuadTree.h"
#include "FileWatcher.h"

//#include "GL/GL.h"
//#include "GL/GLU.h"
//#include "GL/glext.h"
//#include "GL/glcorearb.h"
//#include "GL/glxext.h"
//#include "GL/wglext.h"

//#include "GLAD\glad.h>" // include glad to get all the required OpenGL headers


#include <GL\glew.h>
//#define GLFW_DLL
#include "GLFW\glfw3.h"

//#include <GL\glew.h>
//#include "GLEW\glew.h"
////#define GLFW_DLL
//#include "GLFW\glfw3.h"

//#include <glad/gl.h>
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>

//#include "GL\glfw3.h"
//#include "GL\glfw3native.h"

class RenderWindow {
public:
	int width;
	int height;
	std::string* window_title;
private:
	GLFWwindow* window_context;
	static std::vector<GLFWwindow*>* contexts;
	static int num_contexts;
	bool is_fullscreen;
	AABB* scene_bounds;
	QuadTree* scene_tree;
	Render_QuadTree* render_scene;
	Matrix4* camera;
	Vector3f* camera_pos;
	Quaternion* camera_orientation;
	float camera_angle_pitch = 0.0f;
	float camera_angle_yaw = 0.0f;
	float camera_angle_roll = 0.0f;
	Vector3f* camera_front = new Vector3f(0.0f, 0.0f, 1.0f);
	Vector3f* camera_up = new Vector3f(0.0f, 1.0f, 0.0f);
	float turnSpeed = 0.001f;
	float camera_speed = 0.001f;
	bool show_wireframe = false;
	Vector3f* light_position = new Vector3f(0.0f, 0.0f, -2.0f);
	FileWatcher* fw = nullptr;
public:
	RenderWindow(int width, int height, std::string* window_title, bool isfullscreen);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
public:
	~RenderWindow();
public:
	// BEGIN RENDER LOOP.
	void render_loop();
private: 
	void framebuffer_init();
	void process_input(GLFWwindow* window);
	void framebufer_render_loop();
};

#endif