#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 vertexColor; // specify a color output to the fragment shader
uniform vec4 ourColor;
//uniform mat4 MVP;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * model * view  * vec4(aPos, 1.0);
	//gl_Position = view * vec4(aPos, 1.0);
	//gl_Position = vec4(aPos, 1.0);
	vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color
	vertexColor = ourColor;
}