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

// -------------------- Material Uniforms -------------------- //
layout (std140) uniform Material
{
    vec4 u_diffuse_color;
    float u_specular_exponent;
};

// -------------------- Texture Uniforms -------------------- //
struct TextureMap
{
    sampler2D diffuse_texture;
    sampler2D specular_texuture;
    sampler2D normal_texture;
};

uniform TextureMap u_texture_map;

// -------------------- Vertex IN variables ------------------ //
in VertexData
{
    vec3 position;
    vec3 normal;
    vec2 texture_coordinate;
} i;

// -------------------- OUT variables ------------------ //
out vec4 out_color;

// -------------------- Main --------------------------------- //
void main()
{
    vec3 texture_color = texture(u_texture_map.diffuse_texture, i.texture_coordinate).xyz;
    vec3 object_color = texture_color + u_diffuse_color.xyz;
    out_color = vec4(object_color, 1.0);
}