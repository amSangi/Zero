#version 450
precision highp float;

// -------------------- Vertex IN variables ------------------ //
in VertexData
{
    vec3 model_position;
    vec3 normal;
    vec2 texture_coordinate;
    vec4 shadow_coordinate;
} in_vertex_data;

// -------------------- OUT variables ------------------ //
out vec4 out_color;

// -------------------- Main --------------------------------- //
void main()
{
    out_color = vec4(1.0);
}