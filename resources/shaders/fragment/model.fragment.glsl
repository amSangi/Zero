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
    mat4 u_csm_matrices[kShadowCascadeCount];
    vec4 u_csm_far_bounds[kShadowCascadeCount];
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
    vec3 normal;
    vec2 texture_coordinate;
    vec4 shadow_coordinates[kShadowCascadeCount];
    float clip_space_z_position;
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
        if (IN.clip_space_z_position <= u_csm_far_bounds[cascade_index].z)
        {
            return ComputeCascadeShadowFactor(cascade_index, IN.shadow_coordinates[cascade_index]);
        }
    }
    return 1.0;
}

//////////////////////////////////////////////////
////////// Lighting Functions
//////////////////////////////////////////////////
vec4 ComputeBaseLightColor(vec3 light_color,
                           vec3 light_to_vertex,
                           vec3 vertex_to_eye,
                           vec3 normal,
                           float ambient_intensity,
                           float diffuse_intensity)
{
    // Compute ambient color of the light
    vec4 ambient_color = vec4(light_color * ambient_intensity, 1.0);

    // Compute diffuse color of the light
    float diffuse_factor = max(dot(normal, -light_to_vertex), 0.0);
    vec4 diffuse_color = vec4(light_color * diffuse_intensity * diffuse_factor, 1.0);

    // Compute specular color of the light
    vec3 reflection_direction = normalize(reflect(light_to_vertex, normal));
    float specular_factor = pow(max(dot(vertex_to_eye, reflection_direction),
                                    0.0),
                                u_specular_exponent);

    vec4 specular_color = vec4(light_color, 1.0) * u_specular_intensity * specular_factor;

    return (ambient_color + (diffuse_color + specular_color));
}

vec4 ComputeDirectionalLightColor(DirectionalLight light, vec3 normal, vec3 vertex_to_eye)
{
    return ComputeBaseLightColor(light.color.xyz,
                                 light.direction.xyz,
                                 vertex_to_eye,
                                 normal,
                                 light.ambient_intensity,
                                 light.diffuse_intensity);
}

vec4 ComputePointLightColor(PointLight light, vec3 normal, vec3 vertex_to_eye)
{
    vec3 light_to_vertex = IN.world_position - light.position.xyz;
    float distance = length(light_to_vertex);
    light_to_vertex = normalize(light_to_vertex);

    vec4 base_light_color = ComputeBaseLightColor(light.color.xyz,
                                                  light_to_vertex,
                                                  vertex_to_eye,
                                                  normal,
                                                  light.ambient_intensity,
                                                  light.diffuse_intensity);

    float attenuation = light.attenuation_constant
                        + (light.attenuation_linear * distance)
                        + (light.attenuation_quadratic * distance * distance);

    return base_light_color / attenuation;
}

vec4 ComputeSpotLightColor(SpotLight light, vec3 normal, vec3 vertex_to_eye)
{
    vec3 light_to_vertex = IN.world_position - light.position.xyz;
    float distance = length(light_to_vertex);
    light_to_vertex = normalize(light_to_vertex);

    vec4 base_light_color = ComputeBaseLightColor(light.color.xyz,
                                                  light_to_vertex,
                                                  vertex_to_eye,
                                                  normal,
                                                  light.ambient_intensity,
                                                  light.diffuse_intensity);

    float attenuation = light.attenuation_constant
                        + (light.attenuation_linear * distance)
                        + (light.attenuation_quadratic * distance * distance);

    float theta = dot(light_to_vertex, light.direction.xyz);
    float epsilon = (light.inner_cosine - light.outer_cosine);
    float spot_light_intensity = clamp((theta - light.outer_cosine) / epsilon, 0.0, 1.0);

    return (base_light_color / attenuation) * spot_light_intensity;
}

vec4 ComputeLightColor(vec3 normal, vec3 vertex_to_eye)
{
    vec4 lighting_color = vec4(0, 0, 0, 0);

    // Compute Directional Light Colors
    for (int i = 0; (i < kMaxDirectionalLightCount) && (i < u_directional_light_count); ++i)
    {
        lighting_color += ComputeDirectionalLightColor(u_directional_lights[i], normal, vertex_to_eye);
    }

    // Compute Point Light Colors
    for (int i = 0; (i < kMaxPointLightCount) && (i < u_point_light_count); ++i)
    {
        lighting_color += ComputePointLightColor(u_point_lights[i], normal, vertex_to_eye);
    }

    // Compute Spot Light Colors
    for (int i = 0; (i < kMaxSpotLightCount) && (i < u_spot_light_count); ++i)
    {
        lighting_color += ComputeSpotLightColor(u_spot_lights[i], normal, vertex_to_eye);
    }

    return lighting_color;
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
    vec4 light_color = ComputeLightColor(normal, vertex_to_eye);

    out_color = object_color * light_color * ComputeCascadedShadowMap();
}