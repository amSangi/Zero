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

in vec3 model_view_position;
in vec2 texture_coord;

void main() {
    gl_FragColor = texture2D(diffuse_texture, texture_coord);
}