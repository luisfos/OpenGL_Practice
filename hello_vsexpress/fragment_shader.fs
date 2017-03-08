#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec3 ObjectColor;

out vec4 color;

vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
vec3 lightPos = vec3(1.0f, 3.0f, 1.0f);


void main()
{
	// ambient
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 result = (ambient + diff) * ObjectColor;
    color = vec4(result, 1.0f);
}