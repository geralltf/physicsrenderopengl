#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>
#include <iostream>
#include <istream>
#include <sstream>
#include <fstream>

//#include "GL\glfw3.h"
//#include "GL\GL.h"
//#include "GL\GLU.h"
//#include "GL\glext.h"
//#include "GL\glcorearb.h"
//#include "GL\glxext.h"
//#include "GL\wglext.h"

//#include "GLAD\glad.h>" // include glad to get all the required OpenGL headers
//
#include <GL\glew.h>
//#define GLFW_DLL
#include "GLFW\glfw3.h"

//#include <glad/gl.h>
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>

enum SHADER_TYPE {
	VertexShader,
	FragmentShader
};

class ShaderProgram {
public:
	unsigned int shaderProgram;
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int VBO;
	unsigned int VAO;

	float* vertices;
	bool has_init = false;

	static std::string* read_text_file(std::string* file_name)
	{
		std::ifstream t(file_name->c_str());
		std::stringstream buffer;
		buffer << t.rdbuf();
		std::string* result = new std::string(buffer.str());
		return result;
	}
	ShaderProgram() {
		shaderProgram = glCreateProgram();
	}
	ShaderProgram(std::string* vertex_file_name, std::string* frag_file_name) {
		shaderProgram = glCreateProgram();

		LoadShaderProgram(vertex_file_name, SHADER_TYPE::VertexShader);
		LoadShaderProgram(frag_file_name, SHADER_TYPE::FragmentShader);

		glLinkProgram(shaderProgram);

		//init();	
	}
	~ShaderProgram() {
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(shaderProgram);
	}
	bool LoadShaderProgram(std::string* file_name, SHADER_TYPE shader_type) 
	{
		std::string* shader_source = read_text_file(file_name);

		if (shader_type == SHADER_TYPE::VertexShader)
		{
			
			vertexShader = glCreateShader(GL_VERTEX_SHADER);


			const char* astr = shader_source->c_str();
			const GLchar* bstr = (const GLchar*)(astr);
			const GLchar** glstr = &bstr;
			glShaderSource(vertexShader, 1, glstr, NULL);
			glCompileShader(vertexShader);

			int  success;
			char infoLog[512];
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

			if (!success)
			{
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			}

			glAttachShader(shaderProgram, vertexShader);
		}
		else if (shader_type == SHADER_TYPE::FragmentShader)
		{
			fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

			const char* astr = shader_source->c_str();
			const GLchar* bstr = (const GLchar*)(astr);
			const GLchar** glstr = &bstr;
			glShaderSource(fragmentShader, 1, glstr, NULL);
			glCompileShader(fragmentShader);

			int  success;
			char infoLog[512];
			
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

			if (!success)
			{
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
			}

			glAttachShader(shaderProgram, fragmentShader);
		}

		return true;
	}

public:
	void init()
	{
		if (has_init == false) 
		{
			////////////////
			vertices = new float[9] {
				-0.5f, -0.5f, 0.0f,
					0.5f, -0.5f, 0.0f,
					0.0f, 0.5f, 0.0f
				};


			glGenVertexArrays(1, &VAO);

			glBindVertexArray(VAO);

			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);

			glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices, GL_STATIC_DRAW);
		
			//glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		}
		has_init = true;
	}
	void use(Matrix4* camera, Vector3f* camera_pos, Quaternion* camera_orientation, float cameraAngle) {
		//glUseProgram(shaderProgram);
		//glBindVertexArray(VAO);
		// 0. copy our vertices array in a buffer for OpenGL to use
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices, GL_STATIC_DRAW);
		// 1. then set the vertex attributes pointers
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);




		// 2. use our shader program when we want to render an object
		glUseProgram(shaderProgram);

		// 3. now draw the object 
		// update the uniform color
		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		if (vertexColorLocation != -1) 
		{
			glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		}
		camera = Matrix4::Identity();
		//camera->SetTranslation(camera_pos);
		//camera->Translate(Quaternion::Multiply(camera_orientation, camera_pos->Add(new Vector3f(0.0f, 0.0f, -2.0f))));
		//camera->Translate();
		
		Matrix4* cam_transl = Matrix4::CreateTranslationMatrix(camera_pos->Add(new Vector3f(0.0f, 0.0f, -2.0f)));

		//camera->Scale(new Vector3f(0.1f, 0.1f, 0.1f));
		Matrix4* projection = Matrix4::Perspective(45, 800.0f / 600.0f, 0.0001f, 100.0f);
		Matrix4* model = Matrix4::Identity();

		//Matrix4* lookAt = Matrix4::LookAt(camera_pos->Add(new Vector3f(0.0f, 0.0f, -2.0f)), Vector3f::Forward, Vector3f::Up);
		
		Matrix4* cam_rot = Matrix4::CreateRotatationMatrix(Vector3f::Up, cameraAngle);


		//model = camera;
		camera = Matrix4::Multiply(cam_transl, Matrix4::Multiply(camera, cam_rot));
		//camera = lookAt;

		
		Matrix4* view = camera;
		//Matrix4* view = Matrix4::Multiply(camera, cam_rot); // camera->Inverse();
		//Matrix4* MVP = Matrix4::Multiply(Matrix4::Multiply(view, model), projection);
		//Matrix4* MVP = Matrix4::Multiply(Matrix4::Multiply(projection,view), model);
		Matrix4* MVP = Matrix4::Multiply(Matrix4::Multiply(projection, view), model);

		// glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		//view->Scale(new Vector3f(0.1f, 0.1f, 0.1f));
		float* mat4_arr = MVP->ToArray();
		float* mat4_proj_arr = projection->ToArray();
		float* mat4_view_arr = view->ToArray();
		float* mat4_model_arr = model->ToArray();

		int mvpLocation = glGetUniformLocation(shaderProgram, "MVP");
		if (mvpLocation != -1)
		{
			glUniformMatrix4fv(mvpLocation, 1, false, mat4_arr);
		}

		int projLocation = glGetUniformLocation(shaderProgram, "proj");
		if (projLocation != -1)
		{
			glUniformMatrix4fv(projLocation, 1, false, mat4_proj_arr);
		}

		int viewLocation = glGetUniformLocation(shaderProgram, "view");
		if (viewLocation != -1)
		{
			glUniformMatrix4fv(viewLocation, 1, false, mat4_view_arr);
		}

		int modelLocation = glGetUniformLocation(shaderProgram, "model");
		if (modelLocation != -1)
		{
			glUniformMatrix4fv(modelLocation, 1, false, mat4_model_arr);
		}
		// now render the triangle
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glEnableVertexAttribArray(0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	
		


		delete mat4_arr;
		delete mat4_model_arr;
		delete mat4_proj_arr;
		delete mat4_view_arr;
		//delete projection;
		//delete model;
		//delete view;
		//delete MVP;
	}

};

#endif