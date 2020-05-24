#version 450
precision highp float;

// -------------------- Constants ---------------------------- //
const int kMaxDirectionalLightCount = 8;
const int kMaxPointLightCount = 8;
const int kMaxSpotLightCount = 8;
const float kAmbientStrength = 0.35;

// -------------------- Camera Uniforms ----------------------- //
layout (std140) uniform Camera
{
    mat4 u_projection_matrix;
    mat4 u_view_matrix;
    vec4 u_camera_position;
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
    float intensity;
};

struct PointLight
{
    vec4 color;
    vec4 position;
    float attenuation_constant;
    float attenuation_linear;
    float attenuation_quadratic;
};

struct SpotLight
{
    vec4 color;
    vec4 position;
    vec4 direction;
    float inner_cosine;
    float outer_cosine;
};

layout (std140) uniform Lights
{
    int u_directional_light_count;
    int u_point_light_count;
    int u_spot_light_count;
    DirectionalLight u_directional_lights[kMaxDirectionalLightCount];
    PointLight u_point_lights[kMaxPointLightCount];
    SpotLight u_spot_lights[kMaxSpotLightCount];
};

// -------------------- Material Uniforms -------------------- //
layout (std140) uniform Material
{
    vec4 u_diffuse_color;
    float u_specular_exponent;
};

// -------------------- Texture Uniforms -------------------- //
struct TextureMap
{
    sampler2D diffuse_texture;
    sampler2D specular_texuture;
    sampler2D normal_texture;
};

uniform TextureMap u_texture_map;

// -------------------- Vertex IN variables ------------------ //
in VertexData
{
    vec3 model_position;
    vec3 normal;
    vec2 texture_coordinate;
} i;

// -------------------- OUT variables ------------------ //
out vec4 out_color;


// -------------------- Lighting Functions ------------------- //
vec3 ComputeDirectionalLightColor(DirectionalLight light, vec3 normal, vec3 viewing_direction)
{
    vec3 light_color = light.color.xyz;
    vec3 light_direction = light.direction.xyz;

    // Ambient shading
    vec3 ambient_shading = kAmbientStrength * light_color;

    // Diffuse shading
    vec3 direction = normalize(-light_direction);
    float diffuse_value = max(dot(normal, direction), 0.0);
    vec3 diffuse_shading = diffuse_value * light_color;

    // Specular shading
    vec3 reflection_direction = reflect(-direction, normal);
    float specular_value = pow(max(dot(viewing_direction, reflection_direction), 0.0),
                               u_specular_exponent);
    vec3 specular_shading = specular_value * light_color;

    // Object color
    vec3 texture_color = texture(u_texture_map.diffuse_texture, i.texture_coordinate).xyz;
    vec3 object_color = texture_color + u_diffuse_color.xyz;

    return object_color * (ambient_shading + diffuse_shading + specular_shading);
}

vec3 ComputePointLightColor(PointLight light, vec3 normal, vec3 viewing_direction)
{
    vec3 light_color = light.color.xyz;
    vec3 light_position = light.position.xyz;

    // Ambient shading
    vec3 ambient_color = kAmbientStrength * light_color;

    // Diffuse shading
    vec3 direction = normalize(light_position - i.model_position);
    float diffuse_value = max(dot(normal, direction), 0.0);
    vec3 diffuse_color = diffuse_value * light_color;

    // Specular shading
    vec3 reflection_direction = reflect(-direction, normal);
    float specular_value = pow(max(dot(viewing_direction, reflection_direction), 0.0),
                               u_specular_exponent);
    vec3 specular_color = specular_value * light_color;

    // Attenuation
    float distance = length(light_position - i.model_position);
    float attenuation = 1.0 / (light.attenuation_constant
                                + (light.attenuation_linear * distance)
                                + (light.attenuation_quadratic * distance * distance));

    ambient_color *= attenuation;
    diffuse_color *= attenuation;
    specular_color *= attenuation;

    // Object color
    vec3 texture_color = texture(u_texture_map.diffuse_texture, i.texture_coordinate).xyz;
    vec3 object_color = texture_color + u_diffuse_color.xyz;

    return object_color * (ambient_color + diffuse_color + specular_color);
}

vec3 ComputeSpotLightColor(SpotLight light, vec3 normal, vec3 viewing_direction)
{
    vec3 light_color = light.color.xyz;
    vec3 light_position = light.position.xyz;
    vec3 light_direction = light.direction.xyz;

    // Ambient shading
    vec3 ambient_color = kAmbientStrength * light_color;

    // Diffuse shading
    vec3 direction = normalize(light_position - i.model_position);
    float diffuse_value = max(dot(normal, direction), 0.0);
    vec3 diffuse_color = diffuse_value * light_color;

    // Specular shading
    vec3 reflection_direction = reflect(-direction, normal);
    float specular_value = pow(max(dot(viewing_direction, reflection_direction), 0.0),
                               u_specular_exponent);
    vec3 specular_color = specular_value * light_color;

    // Spot light intensity
    float theta = dot(direction, normalize(-light_direction));
    float epsilon = (light.inner_cosine - light.outer_cosine);
    float intensity = clamp((theta - light.outer_cosine) / epsilon, 0.0, 1.0);

    diffuse_color *= intensity;
    specular_color *= intensity;

    // Object color
    vec3 texture_color = texture(u_texture_map.diffuse_texture, i.texture_coordinate).xyz;
    vec3 object_color = texture_color + u_diffuse_color.xyz;

    return object_color * (ambient_color + diffuse_color + specular_color);
}

vec3 ComputeLightingColor(vec3 normal, vec3 viewing_direction)
{
    vec3 lighting_color = vec3(0, 0, 0);

    // Compute Directional Light Colors
    for (int i = 0; (i < kMaxDirectionalLightCount) && (i < u_directional_light_count); ++i)
    {
        lighting_color += ComputeDirectionalLightColor(u_directional_lights[i], normal, viewing_direction);
    }

    // Compute Point Light Colors
    for (int i = 0; (i < kMaxPointLightCount) && (i < u_point_light_count); ++i)
    {
        lighting_color += ComputePointLightColor(u_point_lights[i], normal, viewing_direction);
    }

    // Compute Spot Light Colors
    for (int i = 0; (i < kMaxSpotLightCount) && (i < u_spot_light_count); ++i)
    {
        lighting_color += ComputeSpotLightColor(u_spot_lights[i], normal, viewing_direction);
    }

    return lighting_color;
}

// -------------------- Main --------------------------------- //
void main()
{
    vec3 normal = normalize(i.normal);
    vec3 viewing_direction = normalize(u_camera_position.xyz - i.model_position);
    out_color = vec4(ComputeLightingColor(normal, viewing_direction), 1.0);
}