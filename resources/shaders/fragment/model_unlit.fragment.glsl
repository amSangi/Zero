#version 450
precision highp float;

// -------------------- Constants ---------------------------- //
const uint kMaxDirectionalLightCount = 4;
const uint kMaxPointLightCount = 4;
const uint kMaxSpotLightCount = 4;

// -------------------- Camera Uniforms ----------------------- //
layout (std140) uniform Camera
{
    mat4 u_projection_matrix;
    mat4 u_view_matrix;
    vec4 u_camera_position;
};

// -------------------- Material Uniforms -------------------- //
layout (std140) uniform Material
{
    vec4 u_diffuse_color;
    float u_specular_intensity;
    float u_specular_exponent;
};

// -------------------- Model Uniforms ----------------------- //
layout (std140) uniform Model
{
    mat4 u_model_matrix;
    mat4 u_normal_matrix;
};

// -------------------- Light Uniforms ----------------------- //
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

// -------------------- Shadow Map Uniforms -------------------- //
layout (std140) uniform ShadowMapInformation
{
    mat4 u_light_matrix;
};
uniform sampler2DShadow u_shadow_map;

// -------------------- Texture Uniforms -------------------- //
uniform sampler2D u_diffuse_texture;

// -------------------- Vertex IN variables ------------------ //
in VertexData
{
    vec3 model_position;
    vec3 normal;
    vec2 texture_coordinate;
    vec4 shadow_coordinate;
} IN;

// -------------------- OUT variables ------------------ //
out vec4 out_color;

// -------------------- Lighting Functions ------------------- //
float ComputeShadowFactor()
{
    float shadow = textureProj(u_shadow_map, IN.shadow_coordinate);
    if (shadow == 0.0)
    {
        return 0.50;
    }
    return 1.0;
}

// -------------------- Main --------------------------------- //
void main()
{
    vec3 normal = normalize(IN.normal);
    vec3 vertex_to_eye = normalize(u_camera_position.xyz - IN.model_position);

    vec3 texture_color = texture(u_diffuse_texture, IN.texture_coordinate).xyz;
    vec3 object_color = texture_color + u_diffuse_color.xyz;

    out_color = vec4(object_color, 1.0) * ComputeShadowFactor();
}