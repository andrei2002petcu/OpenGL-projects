#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Output
layout(location = 0) out vec4 out_color;

// Uniforms
uniform vec3 object_color;
uniform vec3 eye_position;
uniform vec3 light_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

void main()
{
	vec3 N = normalize(world_normal);
	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);

	float ambient_light = 0.45f;
	float diffuse_light = material_kd * max(dot(N, L), 0.0);
	float specular_light = 0;

	if (diffuse_light > 0)
	{
		specular_light = material_ks * pow(max(dot(N, H), 0), material_shininess);
	}

	float light = ambient_light + diffuse_light + specular_light;

	out_color = vec4(object_color * light, 1.0);
}
