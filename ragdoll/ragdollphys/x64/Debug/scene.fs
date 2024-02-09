#version 330 core
out vec4 FragColor;

in FS_IN {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

//in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  
in vec4 outVaryColour;
in vec4 outColor;
in vec3  normCoord;
in vec3 TexCoord;
in mat3 TBN;

uniform sampler2D ourTexture;
uniform sampler2D ourTextureNormals;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    //FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	//FragColor = vertexColor;
	vec3 diffuse_tex = texture(ourTexture, vec2(TexCoord.x,TexCoord.y)).rgb;
	//vec4 result2 = texture2D(ourTextureNormals, vec2(TexCoord.x,TexCoord.y));
	//FragColor = vec4(TexCoord.x,TexCoord.y, TexCoord.z, 1.0);
	//FragColor = vec4(vec3(result.x, result.y, result.z) , 1.0); // + (outVaryColour.rgb)
	
	//FragColor = result;
	
	// obtain normal from normal map in range [0,1]
    //vec3 normal = texture(ourTextureNormals, vec2(TexCoord.x,TexCoord.y)).rgb;
    // transform normal vector to range [-1,1]
    //normal = normalize(normal * 2.0 - 1.0);   
	
	vec3 normal = texture(ourTextureNormals, vec2(TexCoord.x, TexCoord.y)).rgb;
	normal = normal * 2.0 - 1.0;   
	normal = normalize(TBN * normal); 
	
	//vec3 lightPos = vec3(0.0, 0.0, -3.0);
	
	vec3 lightDir = TBN * normalize(lightPos - fs_in.FragPos);
    vec3 viewDir  = TBN * normalize(viewPos - fs_in.FragPos); 
	
	vec3 ambient = vec3(0.2, 0.2, 0.2);
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 objectColor = diffuse_tex.rgb;
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = (diff * lightColor);
	vec3 shader_result = ((ambient + diffuse) * objectColor);
	FragColor = vec4(vec3(shader_result.r, shader_result.g, shader_result.b), 1.0);
	//FragColor = vec4(diffuse_tex.r, diffuse_tex.g, diffuse_tex.b, 1.0);
	//FragColor = vec4(texture(ourTexture, TexCoord.xy).rgb, 1.0);
	//FragColor = ourColor;
}