#version 330 core
layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 color;

// vertex shader first
// runs on every vertex provided
// maybe we do our transforms here? 

out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    ourColor = vec3(0.7f, 0.4f, 0.6f);
}