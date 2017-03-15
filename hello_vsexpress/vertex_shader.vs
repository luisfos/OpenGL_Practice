#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

// vertex shader first
// runs on every vertex provided
// maybe we do our transforms here? 

out vec3 ObjectColor;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	
    gl_Position = projection * view * model * vec4(position, 1.0f);
	
	// outs
	FragPos = vec3(model * vec4(position, 1.0f));
	
	Normal = normal;
    ObjectColor = vec3(0.0f, 0.9f, 0.9f);
}