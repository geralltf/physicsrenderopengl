#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
//layout (location = 2) in vec2 aTexCoord;

out vec4 ourColor;
//out vec3  normCoord;
//out vec2 TexCoord;

//uniform vec4 ourColor;
//uniform mat4 MVP;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);
	//ourColor = vec4(0.5, 0.0, 0.0, 1.0);
    ourColor = vec4(0.3, 0.0, 0.3, 1.0);// vec4(aColor, 1.0);
    //TexCoord = aTexCoord;
}