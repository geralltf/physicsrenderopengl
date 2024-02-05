#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aNorm;

out vec4 vertexColor; // specify a color output to the fragment shader
uniform vec4 ourColor;
//uniform mat4 MVP;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);
	//gl_Position = vec4(aPos, 1.0);
	vertexColor = vec4(0.0, 0.0, 0.3, 1.0); // set the output variable to a dark-green color
	//vertexColor = ourColor;
}