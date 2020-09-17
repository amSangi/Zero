#version 450
precision highp float;

//////////////////////////////////////////////////
////////// Constants
//////////////////////////////////////////////////
const uint kShadowCascadeCount = 3;

//////////////////////////////////////////////////
////////// IN Variables
//////////////////////////////////////////////////
in VertexData
{
    vec3 model_position;
    vec3 normal;
    vec2 texture_coordinate;
    vec4 shadow_coordinates[kShadowCascadeCount];
    float clip_space_z_position;
} in_vertex_data;

//////////////////////////////////////////////////
////////// OUT Variables
//////////////////////////////////////////////////
out vec4 out_color;

//////////////////////////////////////////////////
////////// Main
//////////////////////////////////////////////////
void main()
{
    out_color = vec4(1.0);
}