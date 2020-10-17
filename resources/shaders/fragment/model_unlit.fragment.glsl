#version 450
precision highp float;

//////////////////////////////////////////////////
////////// Constants
//////////////////////////////////////////////////
const uint kMaxDirectionalLightCount = 4;
const uint kMaxPointLightCount = 4;
const uint kMaxSpotLightCount = 4;
const uint kShadowCascadeCount = 3;

//////////////////////////////////////////////////
////////// Camera Uniforms
//////////////////////////////////////////////////
layout (std140) uniform Camera
{
    mat4 u_projection_matrix;
    mat4 u_view_matrix;
    vec4 u_camera_position;
};

//////////////////////////////////////////////////
////////// Material Uniforms
//////////////////////////////////////////////////
layout (std140) uniform Material
{
    vec4 u_diffuse_color;
    float u_specular_intensity;
    float u_specular_exponent;
};

//////////////////////////////////////////////////
////////// Model Uniforms
//////////////////////////////////////////////////
layout (std140) uniform Model
{
    mat4 u_model_matrix;
    mat4 u_normal_matrix;
};

//////////////////////////////////////////////////
////////// Light Uniforms
//////////////////////////////////////////////////
struct DirectionalLight
{
    vec4 color;
    vec4 direction;
    float ambient_intensity;
    float diffuse_intensity;
};

struct PointLight
{
    vec4 position;
    vec4 color;

    float ambient_intensity;
    float diffuse_intensity;
    float attenuation_constant;
    float attenuation_linear;

    float attenuation_quadratic;
};

struct SpotLight
{
    vec4 position;
    vec4 color;
    vec4 direction;

    float ambient_intensity;
    float diffuse_intensity;
    float inner_cosine;
    float outer_cosine;

    float attenuation_constant;
    float attenuation_linear;
    float attenuation_quadratic;
};

layout (std140) uniform LightInformation
{
    uint u_directional_light_count;
    uint u_point_light_count;
    uint u_spot_light_count;
};

layout (std140) uniform DirectionalLights
{
    DirectionalLight u_directional_lights[kMaxDirectionalLightCount];
};

layout (std140) uniform PointLights
{
    PointLight u_point_lights[kMaxPointLightCount];
};

layout (std140) uniform SpotLights
{
    SpotLight u_spot_lights[kMaxSpotLightCount];
};

//////////////////////////////////////////////////
////////// Shadow Map Uniforms
//////////////////////////////////////////////////
layout (std140) uniform ShadowMapInformation
{
    mat4 u_csm_texture_matrices[kShadowCascadeCount];
    vec4 u_csm_view_far_bounds[kShadowCascadeCount];
};
uniform sampler2DShadow u_cascaded_shadow_map[kShadowCascadeCount];

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
////////// Shadow Function
//////////////////////////////////////////////////
float ComputeCascadeShadowFactor(uint cascade_index, vec4 shadow_coordinate)
{
    float shadow_factor = textureProj(u_cascaded_shadow_map[cascade_index], shadow_coordinate);
    if (shadow_factor == 0.0)
    {
        return 0.65;
    }
    return 1.0;
}

float ComputeCascadedShadowMap()
{
    for (uint cascade_index = 0; cascade_index < kShadowCascadeCount; ++cascade_index)
    {
        if (IN.view_position.z < u_csm_view_far_bounds[cascade_index].z)
        {
            return ComputeCascadeShadowFactor(cascade_index, IN.shadow_coordinates[cascade_index]);
        }
    }
    return 1.0;
}

//////////////////////////////////////////////////
////////// Main
//////////////////////////////////////////////////
void main()
{
    vec3 normal = normalize(IN.normal);
    vec3 vertex_to_eye = normalize(u_camera_position.xyz - IN.world_position);

    vec3 texture_color = texture(u_diffuse_texture, IN.texture_coordinate).xyz;
    vec4 object_color = vec4(texture_color + u_diffuse_color.xyz, 1.0);

    out_color = object_color * ComputeCascadedShadowMap();
}