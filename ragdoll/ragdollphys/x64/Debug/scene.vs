#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec3 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

out vec4 outVaryColour;
out vec4 outColor;
out vec3  normCoord;
out vec3 TexCoord;
out mat3 TBN;

uniform vec4 ourColor;
//uniform mat4 MVP;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);
	//ourColor = vec4(0.5, 0.0, 0.0, 1.0);
    outColor = vec4(0.3, 0.0, 0.0, 1.0);// vec4(aNorm, 1.0);
	outVaryColour = ourColor;
	normCoord = aNorm;
    TexCoord = aTexCoord; //vec2(0.5,0.5);// aTexCoord;
	
	vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
	vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(aNorm,    0.0)));
	TBN = mat3(T, B, N);
	TBN = transpose(mat3(T, B, N)); 
   
	vs_out.TangentLightPos = TBN * lightPos;
	vs_out.TangentViewPos  = TBN * viewPos;
	vs_out.TangentFragPos  = TBN * vec3(model * vec4(aPos, 1.0));
}