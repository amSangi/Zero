#version 450
precision highp float;

// -------------------- Material Uniforms -------------------- //
struct Material {
    sampler2D diffuse_texture;
    sampler2D specular_texuture;
    sampler2D normal_texture;
    vec3 diffuse_color;
    float specular_exponent;
};

uniform Material material;

// -------------------- Model Uniforms ----------------------- //
uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;
uniform mat3 normal_matrix;
uniform vec3 camera_position;

// -------------------- Vertex IN variables ------------------ //
in vec3 model_view_position;
in vec3 transformed_normal;
in vec2 texture_coord;

// -------------------- OUT variables ------------------ //
out vec4 final_color;

// -------------------- Main --------------------------------- //
void main() {
    vec3 texture_color = texture2D(material.diffuse_texture, texture_coord).xyz;
    vec3 object_color = texture_color + material.diffuse_color;
    final_color = vec4(object_color, 1.0);
}