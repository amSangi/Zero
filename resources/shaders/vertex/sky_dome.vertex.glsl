#version 450
precision highp float;

// -------------------- Uniforms ----------------------- //
uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;
uniform vec4 u_camera_position;

// -------------------- IN variables ------------------ //
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texture_coordinate;

// -------------------- OUT variables ------------------ //
out VertexData
{
    vec3 model_position;
    vec3 normal;
    vec2 texture_coordinate;
} o;

void main()
{
    o.texture_coordinate = in_texture_coordinate;

    vec4 model_position_4D = (u_model_matrix * vec4(in_position, 1));
    o.model_position = model_position_4D.xyz;

    // Model matrix will always be a simple translation
    vec4 normal_4D = (u_model_matrix * vec4(in_normal, 1));
    o.normal = normal_4D.xyz;

    gl_Position = (u_projection_matrix * (u_view_matrix * model_position_4D));
}
