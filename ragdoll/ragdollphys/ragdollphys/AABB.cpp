#include "AABB.h"

AABB::AABB() {
	center = Vector2f_ZERO;
	half_dimension = 0.5f;
}
AABB::AABB(const Vector2f* _center, const float _half_dimension) {
	center = (Vector2f*)_center;
	half_dimension = _half_dimension;
}
bool AABB::contains_point(const Vector2f* point) {
	return true;
}
bool AABB::intersects_AABB(const AABB* other) {
	return true;
}
void AABB::render(Matrix4* camera, Vector3f* camera_pos, Quaternion* camera_orientation, float camera_angle_pitch, float camera_angle_yaw, float camera_angle_roll, Vector3f** camera_front, Vector3f** camera_up, bool show_wireframe) {

	//float width = 10.0f;
	//float height = 10.0f;
	//float depth = 10.0f;
	//float vertices[] = { -width, -height, -depth, // 0
	//						  width, -height, -depth, // 1
	//						  width,  height, -depth, // 2
	//						 -width,  height, -depth, // 3
	//						 -width, -height,  depth, // 4
	//						  width, -height,  depth, // 5
	//						  width,  height,  depth, // 6
	//						 -width,  height,  depth // 7
	//};
	//uint32_t indices[] = { 
	//	0, 2, 1,
	//	0, 3, 2,

	//	1,2,6,
	//	6,5,1,

	//	4,5,6,
	//	6,7,4,

	//	2,3,6,
	//	6,3,7,

	//	0,7,3,
	//	0,4,7,

	//	0,1,5,
	//	0,5,4
	//};

	if (has_init == false)
	{
		shader_program = new ShaderProgram(new std::string("aabb.vs"),new std::string("aabb.fs"));

		//float w = 0.5f;
		//float h = 0.5f;
		//float d = 0.5f;
		//Vector3f* _center = new Vector3f(0.0f,0.0f,0.0f);
		//Vector3f* boxdims = new Vector3f();
		//boxdims->x = w;
		//boxdims->y = h;
		//boxdims->z = d;

		//GLfloat* vboverticies = new GLfloat[72];
		//GLfloat* vbonormals = new GLfloat[18];
		//Vector3f** verticies;
		//
		//verticies = (Vector3f**)new Vector3f[24];

		////Top face
		//verticies[0] = new Vector3f(-boxdims->x / 2.0f, boxdims->y / 2.0f, -boxdims->z / 2.0f);
		//verticies[1] = new Vector3f(-boxdims->x / 2.0f, boxdims->y / 2.0f, boxdims->z / 2.0f);
		//verticies[2] = new Vector3f(boxdims->x / 2.0f, boxdims->y / 2.0f, boxdims->z / 2.0f);
		//verticies[3] = new Vector3f(boxdims->x / 2.0f, boxdims->y / 2.0f, -boxdims->z / 2.0f);


		////Bottom face
		//verticies[4] = new Vector3f(-boxdims->x / 2.0f, -boxdims->y / 2.0f, -boxdims->z / 2.0f);
		//verticies[5] = new Vector3f(boxdims->x / 2.0f, -boxdims->y / 2.0f, -boxdims->z / 2.0f);
		//verticies[6] = new Vector3f(boxdims->x / 2.0f, -boxdims->y / 2.0f, boxdims->z / 2.0f);
		//verticies[7] = new Vector3f(-boxdims->x / 2.0f, -boxdims->y / 2.0f, boxdims->z / 2.0f);


		////Left face
		//verticies[8] = new Vector3f(-boxdims->x / 2.0f, -boxdims->y / 2.0f, -boxdims->z / 2.0f);
		//verticies[9] = new Vector3f(-boxdims->x / 2.0f, -boxdims->y / 2.0f, boxdims->z / 2.0f);
		//verticies[10] = new Vector3f(-boxdims->x / 2.0f, boxdims->y / 2.0f, boxdims->z / 2.0f);
		//verticies[11] = new Vector3f(-boxdims->x / 2.0f, boxdims->y / 2.0f, -boxdims->z / 2.0f);


		////Right face
		//verticies[12] = new Vector3f(boxdims->x / 2.0f, -boxdims->y / 2.0f, -boxdims->z / 2.0f);
		//verticies[13] = new Vector3f(boxdims->x / 2.0f, boxdims->y / 2.0f, -boxdims->z / 2.0f);
		//verticies[14] = new Vector3f(boxdims->x / 2.0f, boxdims->y / 2.0f, boxdims->z / 2.0f);
		//verticies[15] = new Vector3f(boxdims->x / 2.0f, -boxdims->y / 2.0f, boxdims->z / 2.0f);

		////Front face
		//verticies[16] = new Vector3f(-boxdims->x / 2.0f, -boxdims->y / 2.0f, boxdims->z / 2.0f);
		//verticies[17] = new Vector3f(boxdims->x / 2.0f, -boxdims->y / 2.0f, boxdims->z / 2.0f);
		//verticies[18] = new Vector3f(boxdims->x / 2.0f, boxdims->y / 2.0f, boxdims->z / 2.0f);
		//verticies[19] = new Vector3f(-boxdims->x / 2.0f, boxdims->y / 2.0f, boxdims->z / 2.0f);

		////Back face
		//verticies[20] = new Vector3f(-boxdims->x / 2.0f, -boxdims->y / 2.0f, -boxdims->z / 2.0f);
		//verticies[21] = new Vector3f(-boxdims->x / 2.0f, boxdims->y / 2.0f, -boxdims->z / 2.0f);
		//verticies[22] = new Vector3f(boxdims->x / 2.0f, boxdims->y / 2.0f, -boxdims->z / 2.0f);
		//verticies[23] = new Vector3f(boxdims->x / 2.0f, -boxdims->y / 2.0f, -boxdims->z / 2.0f);




		//for (int i = 0; i < 24; i++)
		//{
		//	verticies[i]->x += _center->x;
		//	verticies[i]->y += _center->y;
		//	verticies[i]->z += _center->z;
		//}


		//int count = 0;
		//for (int i = 0; i < 24; ++i)
		//{
		//	vboverticies[count] = verticies[i]->x;
		//	count++;
		//	vboverticies[count] = verticies[i]->y;
		//	count++;
		//	vboverticies[count] = verticies[i]->z;
		//	count++;
		//}

		////glNormal3f(0.0, 1.0f, 0.0f);
		////glNormal3f(0.0, -1.0f, 0.0f);
		////glNormal3f(-1.0, 0.0f, 0.0f);
		////glNormal3f(1.0, 0.0f, 0.0f);
		////glNormal3f(0.0, 0.0f, 1.0f);
		////glNormal3f(0.0, 0.0f, -1.0f);
		//vbonormals[0] = (0.0);
		//vbonormals[1] = (1.0);
		//vbonormals[2] = (0.0);

		//vbonormals[3] = (0.0);
		//vbonormals[4] = (-1.0);
		//vbonormals[5] = (0.0);

		//vbonormals[6] = (-1.0);
		//vbonormals[7] = (0.0);
		//vbonormals[8] = (0.0);

		//vbonormals[9] = (1.0);
		//vbonormals[10] = (0.0);
		//vbonormals[11] = (0.0);

		//vbonormals[12] = (0.0);
		//vbonormals[13] = (0.0);
		//vbonormals[14] = (1.0);

		//vbonormals[15] = (0.0);
		//vbonormals[16] = (0.0);
		//vbonormals[17] = (-1.0);


		//glGenVertexArrays(1, &VAO);

		//glBindVertexArray(VAO);
 
		////Create the VBO
		//glDeleteBuffers(1, &VBO);
		//glGenBuffers(1, &VBO);
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glBufferData(GL_ARRAY_BUFFER, (72 * sizeof(GLfloat)) + (18 * sizeof(GLfloat)), NULL, GL_STATIC_COPY);

		//glBufferSubData(GL_ARRAY_BUFFER, 0, 72 * sizeof(GLfloat), vboverticies);
		//glBufferSubData(GL_ARRAY_BUFFER, 72 * sizeof(GLfloat), 18 * sizeof(GLfloat), vbonormals);



		static const GLfloat g_vertex_buffer_data[] = {
			-1.0f,-1.0f,-1.0f, // triangle 1 : begin
			-1.0f,-1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f, // triangle 1 : end
			1.0f, 1.0f,-1.0f, // triangle 2 : begin
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f,-1.0f, // triangle 2 : end
			1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f,-1.0f, 1.0f,
			1.0f,-1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f,-1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f,-1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f,-1.0f, 1.0f
		};




		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, 12 * 3 * sizeof(float), g_vertex_buffer_data, GL_STATIC_DRAW);
		
		//glGenBuffers(1, &EBO);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(uint32_t), indices, GL_STATIC_DRAW);

		////glGenVertexArrays(1, &VAO);
		//glBindVertexArray(VAO);
		//glEnableVertexAttribArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		///*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);*/

		shader_program->has_init = true;
		has_init = true;


	}
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glEnableVertexAttribArray(0);




	// 2. use our shader program when we want to render an object
	glUseProgram(shader_program->shaderProgram);

	// 3. now draw the object 
	// update the uniform color
	float timeValue = glfwGetTime();
	float greenValue = sin(timeValue) / 2.0f + 0.5f;
	int vertexColorLocation = glGetUniformLocation(shader_program->shaderProgram, "ourColor");
	if (vertexColorLocation != -1)
	{
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
	}
	camera = Matrix4::Identity();
	Vector3f* object_pos = new Vector3f(0.0f, 0.5f, 0.0f);
	object_pos = object_pos->Add(camera_pos);
	//camera->SetTranslation(camera_pos);
	camera->Translate(object_pos->Add(new Vector3f(0.0f, 0.0f, -2.0f)));
	//camera->Scale(new Vector3f(0.1f, 0.1f, 0.1f));
	Matrix4* projection = Matrix4::Perspective(45, 800.0f / 600.0f, 0.0001f, 100.0f);
	Matrix4* model = Matrix4::Identity();
	//model->Translate(object_pos);
	Matrix4* view = camera; // camera->Inverse();
	//Matrix4* MVP = Matrix4::Multiply(Matrix4::Multiply(view, model), projection);
	//Matrix4* MVP = Matrix4::Multiply(Matrix4::Multiply(projection,view), model);
	Matrix4* MVP = Matrix4::Multiply(Matrix4::Multiply(projection, view), model);

	// glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
	//view->Scale(new Vector3f(0.1f, 0.1f, 0.1f));
	float* mat4_arr = MVP->ToArray();
	float* mat4_proj_arr = projection->ToArray();
	float* mat4_view_arr = view->ToArray();
	float* mat4_model_arr = model->ToArray();

	int mvpLocation = glGetUniformLocation(shader_program->shaderProgram, "MVP");
	if (mvpLocation != -1)
	{
		glUniformMatrix4fv(mvpLocation, 1, false, mat4_arr);
	}

	int projLocation = glGetUniformLocation(shader_program->shaderProgram, "proj");
	if (projLocation != -1)
	{
		glUniformMatrix4fv(projLocation, 1, false, mat4_proj_arr);
	}

	int viewLocation = glGetUniformLocation(shader_program->shaderProgram, "view");
	if (viewLocation != -1)
	{
		glUniformMatrix4fv(viewLocation, 1, false, mat4_view_arr);
	}

	int modelLocation = glGetUniformLocation(shader_program->shaderProgram, "model");
	if (modelLocation != -1)
	{
		glUniformMatrix4fv(modelLocation, 1, false, mat4_model_arr);
	}
	// now render the triangle
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glEnableVertexAttribArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, (show_wireframe? GL_LINE : GL_FILL));
	//glDrawElements(GL_TRIANGLES, 0, 36, &indices);
	//glDrawElements(GL_TRIANGLES, 36,  GL_UNSIGNED_INT, &indices);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);



	delete mat4_arr;
	delete mat4_model_arr;
	delete mat4_proj_arr;
	delete mat4_view_arr;
	//delete projection;
	//delete model;
	//delete view;
	//delete MVP;

}