#version 450
precision highp float;

//////////////////////////////////////////////////
////////// Camera Uniforms
//////////////////////////////////////////////////
layout (std140) uniform Camera
{
    mat4 u_projection_matrix;
    mat4 u_view_matrix;
    vec4 u_camera_position;
};

//////////////////////////////////////////////////
////////// Model Uniforms
//////////////////////////////////////////////////
layout (std140) uniform Model
{
    mat4 u_model_matrix;
    mat4 u_normal_matrix;
};

//////////////////////////////////////////////////
////////// IN Variables
//////////////////////////////////////////////////
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texture_coordinate;

//////////////////////////////////////////////////
////////// OUT Variables
//////////////////////////////////////////////////
out VertexData
{
    vec3 world_position;
    vec3 normal;
    vec2 texture_coordinate;
} OUT;

//////////////////////////////////////////////////
////////// Main
//////////////////////////////////////////////////
void main()
{
    OUT.texture_coordinate = in_texture_coordinate;

    vec4 world_position_4D = (u_model_matrix * vec4(in_position, 1));
    OUT.world_position = world_position_4D.xyz;

    // Model matrix will always have a uniform scale
    vec4 normal_4D = (u_model_matrix * vec4(in_normal, 1));
    OUT.normal = normal_4D.xyz;

    gl_Position = (u_projection_matrix * (u_view_matrix * world_position_4D));
}
