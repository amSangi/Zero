#version 450
precision highp float;

//////////////////////////////////////////////////
////////// Camera and Model Uniforms
//////////////////////////////////////////////////
uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;
uniform vec4 u_camera_position;

//////////////////////////////////////////////////
////////// Sky Dome Uniforms
//////////////////////////////////////////////////
uniform vec3 u_apex_color;
uniform vec3 u_center_color;

//////////////////////////////////////////////////
////////// IN Variables
//////////////////////////////////////////////////
in VertexData
{
    vec3 world_position;
    vec3 normal;
    vec2 texture_coordinate;
} IN;

//////////////////////////////////////////////////
////////// OUT Variables
//////////////////////////////////////////////////
out vec4 out_color;

//////////////////////////////////////////////////
////////// Main
//////////////////////////////////////////////////
void main()
{
    float height = max(normalize(IN.world_position).y, 0.0);
    vec3 color = mix(u_center_color, u_apex_color, height);
    out_color = vec4(color, 1.0);
}