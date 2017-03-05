#version 330 core
in vec3 ourColor;

out vec4 color;

vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

void main()
{
	// ambient
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;

	vec3 result = ambient * ourColor;
    color = vec4(result, 1.0f);
}