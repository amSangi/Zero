#version 450
precision highp float;

//////////////////////////////////////////////////
////////// Constants
//////////////////////////////////////////////////
const uint kShadowCascadeCount = 3;

//////////////////////////////////////////////////
////////// Texture Uniforms
//////////////////////////////////////////////////
uniform sampler2D u_diffuse_texture;

//////////////////////////////////////////////////
////////// IN Variables
//////////////////////////////////////////////////
in VertexData
{
    vec3 world_position;
    vec3 view_position;
    vec3 normal;
    vec2 texture_coordinate;
    vec4 shadow_coordinates[kShadowCascadeCount];
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
    float alpha = texture(u_diffuse_texture, IN.texture_coordinate).a;
    if (alpha < 0.5)
    {
        discard; 
    }
    out_color = vec4(1.0);
}