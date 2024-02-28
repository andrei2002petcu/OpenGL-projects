#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_texture;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform int health;

// Output
out vec3 world_position;
out vec3 world_normal;

vec3 rand(vec3 pos, int health)
{   
   return v_position + (tan((2 - health * 0.2f) * fract(v_position.z * v_position.x)) * vec3(0.02f, 0.05f, 0.01f));
}

void main()
{
    world_position = (Model * vec4(v_position, 1.0)).xyz;
    world_normal = normalize(mat3(Model) * normalize(v_normal));

    if (health == -1 || health == 4)
        gl_Position = Projection * View * Model * vec4(v_position, 1.0);
    else {
        gl_Position = Projection * View * Model * vec4(rand(v_position, health), 1.0);
    }
}
