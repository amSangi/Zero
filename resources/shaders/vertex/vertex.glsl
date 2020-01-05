#version 450
precision highp float;

uniform sampler2D alpha_texture;
uniform sampler2D ambient_texture;
uniform sampler2D diffuse_texture;
uniform sampler2D displacement_texture;
uniform sampler2D normal_texture;
uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;
uniform vec3 camera_position;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture_coordinate;

out vec3 model_view_position;
out vec2 texture_coord;

void main() {
    texture_coord = texture_coordinate;
    
    vec4 mv_position = model_view_matrix * vec4(position, 1.0);
    model_view_position = mv_position.xyz;
    gl_Position = projection_matrix * mv_position;
}
