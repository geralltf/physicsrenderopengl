#include "RenderWindow.h"

std::vector<GLFWwindow*>* RenderWindow::contexts = nullptr;
int RenderWindow::num_contexts = 0;

RenderWindow::RenderWindow(int width, int height, std::string* window_title, bool isfullscreen) {
	this->width = width;
	this->height = height;
	this->window_title = window_title;
	this->is_fullscreen = isfullscreen;
	this->camera = Matrix4::Identity();
	this->camera_orientation = Quaternion::Identity();
	this->camera_pos = Vector3f::ZERO;
	num_contexts++;

	if (contexts == nullptr) {
		contexts = new std::vector<GLFWwindow*>();
	}

	glfwInit();
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	window_context = glfwCreateWindow(this->width, this->height, window_title->c_str(), isfullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

	if (window_context == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	RenderWindow::contexts->insert(RenderWindow::contexts->begin(), window_context);
	glfwMakeContextCurrent(window_context);
	glViewport(0, 0, this->width, this->height);
	glfwSetFramebufferSizeCallback(window_context, framebuffer_size_callback);
}
void RenderWindow::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
RenderWindow::~RenderWindow() {
	glfwTerminate();
}
// BEGIN RENDER LOOP.
void RenderWindow::render_loop()
{
	framebuffer_init();
	// RENDER LOOP.
	while (!glfwWindowShouldClose(window_context))
	{
		process_input(window_context);
		framebufer_render_loop();
		glfwSwapBuffers(window_context);
		glfwPollEvents();
	}
}
void RenderWindow::framebuffer_init()
{
	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	std::cout << "Renderer: " << renderer << " ";
	std::cout << "OpenGL version supported: " << version << std::endl;

	AABB* scene_bounds = new AABB(Vector2f_ZERO, 5.0f);
	QuadTree* scene_tree = new QuadTree(scene_bounds);
	scene_tree->boundary = scene_bounds;
	//scene_tree->subdivide();

	render_scene = new Render_QuadTree();
	render_scene->init(scene_tree);
}
void RenderWindow::process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		std::cout << "QUIT." << std::endl;
		glfwSetWindowShouldClose(window, true);
	}
	const float camera_speed = 0.001f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera_pos = camera_pos->Add(new Vector3f(0.0f, 0.0f, camera_speed * 1.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera_pos = camera_pos->Add(new Vector3f(0.0f, 0.0f, -camera_speed * 1.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		camera_pos = camera_pos->Add(new Vector3f(0.0f, camera_speed * 1.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		camera_pos = camera_pos->Add(new Vector3f(0.0f, -camera_speed * 1.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		//Vector3f* delta;
		//delta = Vector3f::Cross(new Vector3f(0.0f, 0.0f, camera_speed * 1.0f), Vector3f::Up);
		//delta = delta->Normalised();
		//camera_pos = camera_pos->Add(new Vector3f(delta->x * camera_speed, delta->y * camera_speed, delta->z * camera_speed));
		camera_pos = camera_pos->Add(new Vector3f(camera_speed * 1.0f, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		//Vector3f* delta;
		//delta = Vector3f::Cross(new Vector3f(0.0f, 0.0f, camera_speed * -1.0f), Vector3f::Up);
		//delta = delta->Normalised();
		camera_pos = camera_pos->Add(new Vector3f(-camera_speed * 1.0f, 0.0f, 0.0f));
		//camera_pos = camera_pos->Add(new Vector3f(delta->x * camera_speed, delta->y * camera_speed, delta->z * camera_speed));
		//camera_pos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		camera_orientation = Quaternion::Euler(0.0f, camera_angle, 0.0f);
		camera_angle+=camera_speed * 1.0f;
		//camera->RotateY(camera_angle);

		//float _upAngle = camera_angle;
		//float _rightAngle = 0.0f;
		//Quaternion* q = Quaternion::Euler(_upAngle, 0.0f, 0.0f);
		//Quaternion* q2 = Quaternion::Euler(0.0f, _rightAngle, 0.0f);
		//Quaternion* qtotal = Quaternion::Multiply(q, q2);
		//Matrix4* mat = Matrix4::CreateRotatationMatrix(qtotal);
		//camera = Matrix4::Multiply(camera, mat);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		camera_orientation = Quaternion::Euler(0.0f, camera_angle, 0.0f);
		camera_angle-= camera_speed * 1.0f;
		//camera->RotateY(camera_angle);

		//float _upAngle = camera_angle;
		//float _rightAngle = 0.0f;
		//Quaternion* q = Quaternion::Euler(_upAngle, 0.0f, 0.0f);
		//Quaternion* q2 = Quaternion::Euler(0.0f, _rightAngle, 0.0f);
		//Quaternion* qtotal = Quaternion::Multiply(q, q2);
		//Matrix4* mat = Matrix4::CreateRotatationMatrix(qtotal);
		//camera = Matrix4::Multiply(camera, mat);
	}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		is_fullscreen = !is_fullscreen;

		if (is_fullscreen) 
		{
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);

			glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		}
		else 
		{
			glfwSetWindowMonitor(window, NULL, 0, 0, 800, 600, 0);
		}
	}

	//std::cout << "x:" << camera_pos->x << "y:" << camera_pos->y << "z:" << camera_pos->z << std::endl;
}
void RenderWindow::framebufer_render_loop()
{
	glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	render_scene->render(camera, camera_pos, camera_orientation, camera_angle);
}