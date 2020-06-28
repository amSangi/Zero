#version 450
precision highp float;

// -------------------- Uniforms ----------------------- //
uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;
uniform vec4 u_camera_position;

// -------------------- Vertex IN variables ------------------ //
in VertexData
{
    vec3 model_position;
    vec3 normal;
    vec2 texture_coordinate;
} i;

// -------------------- OUT variables ------------------ //
out vec4 out_color;

// -------------------- Main --------------------------------- //
void main()
{
    out_color = vec4(1, 1, 1, 1);
}