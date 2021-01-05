#version 450
precision highp float;

//////////////////////////////////////////////////
////////// Constants
//////////////////////////////////////////////////
const uint kShadowCascadeCount = 3;

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
////////// Shadow Map Uniforms
//////////////////////////////////////////////////
layout (std140) uniform ShadowMapInformation
{
    mat4 u_csm_texture_matrices[kShadowCascadeCount];
    vec4 u_csm_view_far_bounds[kShadowCascadeCount];
};

//////////////////////////////////////////////////
////////// IN Variables
//////////////////////////////////////////////////
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texture_coordinate;
layout (location = 3) in vec4 in_bone_ids;
layout (location = 4) in vec4 in_bone_weights;

//////////////////////////////////////////////////
////////// OUT Variables
//////////////////////////////////////////////////
out VertexData
{
    vec3 world_position;
    vec3 view_position;
    vec3 normal;
    vec2 texture_coordinate;
    vec4 shadow_coordinates[kShadowCascadeCount];
} OUT;

//////////////////////////////////////////////////
////////// Main
//////////////////////////////////////////////////
void main()
{
    // Compute world space position
    vec4 world_position_4D = (u_model_matrix * vec4(in_position, 1));

    // Extract normal transformation
    mat3 normal_matrix_3 = mat3(u_normal_matrix[0].xyz, u_normal_matrix[1].xyz, u_normal_matrix[2].xyz);

    vec4 view_position = u_view_matrix * world_position_4D;
    gl_Position = (u_projection_matrix * view_position);

    // Set output variables
    OUT.world_position = world_position_4D.xyz;
    OUT.normal = normal_matrix_3 * in_normal;
    OUT.texture_coordinate = in_texture_coordinate;
    for (uint i = 0; i < kShadowCascadeCount; ++i)
    {
        OUT.shadow_coordinates[i] = u_csm_texture_matrices[i] * world_position_4D;
    }
    OUT.view_position = view_position.xyz;
}
