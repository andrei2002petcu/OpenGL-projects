#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_color;
layout(location = 2) in vec3 v_texture;
layout(location = 3) in vec3 v_normal;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_position;
out vec3 frag_color;
out vec3 frag_normal;
out vec3 frag_texture;

float rand(vec3 pos)
{
    //return fract(dot(pos.xy, pos.xz) + pos.z);
    return fract(pos.x * 5.22f + pos.y * 2 + pos.z);
}

void main()
{
    // TODO(student): Send output to fragment shader
    frag_position = v_position;
    frag_color = v_color;
    frag_normal = v_normal;
    frag_texture = v_texture;

    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(v_position * (1 + rand(frag_position) * cos(time + 3 * rand(frag_position))), 1.0);
}
