#version 450
precision highp float;

// -------------------- Vertex IN variables ------------------ //
in VertexData
{
    vec3 model_position;
    vec3 normal;
    vec2 texture_coordinate;
} in_vertex_data;

in vec4 shadow_coordinate;

// -------------------- OUT variables ------------------ //
out vec4 out_color;

// -------------------- Main --------------------------------- //
void main()
{
    out_color = vec4(gl_FragCoord.z);
}