#version 450
precision highp float;

// -------------------- Camera Uniforms ----------------------- //
layout (std140) uniform Camera
{
    mat4 u_projection_matrix;
    mat4 u_view_matrix;
    vec4 u_camera_position;
};

// -------------------- Model Uniforms ----------------------- //
layout (std140) uniform Model
{
    mat4 u_model_matrix;
    mat4 u_normal_matrix;
};

// -------------------- Shadow Map Uniforms -------------------- //
layout (std140) uniform ShadowMapInformation
{
    mat4 u_light_matrix;
};

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
    vec4 shadow_coordinate;
} OUT;

void main()
{
    // Compute world space position
    vec4 model_position_4D = (u_model_matrix * vec4(in_position, 1));

    // Extract normal transformation
    mat3 normal_matrix_3 = mat3(u_normal_matrix[0].xyz, u_normal_matrix[1].xyz, u_normal_matrix[2].xyz);

    // Set output variables
    OUT.model_position = model_position_4D.xyz;
    OUT.normal = normal_matrix_3 * in_normal;
    OUT.texture_coordinate = in_texture_coordinate;
    OUT.shadow_coordinate = u_light_matrix * model_position_4D;

    // Set final vertex position
    gl_Position = (u_projection_matrix * (u_view_matrix * model_position_4D));
}
