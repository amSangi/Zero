#version 450
precision highp float;

// -------------------- Constants ---------------------------- //

// Light Counts
const int kMaxDirectionalLightCount = 8;
const int kMaxPointLightCount = 8;
const int kMaxSpotLightCount = 8;

const float kAmbientStrength = 0.35;

// -------------------- Light Uniforms ----------------------- //
struct DirectionalLight {
    vec3 color;
    vec3 direction;
    float intensity;
};

struct PointLight {
    vec3 color;
    vec3 position;
    float attenuation_constant;
    float attenuation_linear;
    float attenuation_quadratic;
};

struct SpotLight {
    vec3 color;
    vec3 position;
    vec3 direction;
    float inner_cosine;
    float outer_cosine;
};

uniform int directional_light_count;
uniform int point_light_count;
uniform int spot_light_count;
uniform DirectionalLight directional_lights[kMaxDirectionalLightCount];
uniform PointLight point_lights[kMaxPointLightCount];
uniform SpotLight spot_lights[kMaxSpotLightCount];

// -------------------- Material Uniforms -------------------- //
struct Material {
    sampler2D diffuse_texture;
    sampler2D specular_texuture;
    sampler2D normal_texture;
    vec3 diffuse_color;
    float specular_exponent;
};

uniform Material material;

// -------------------- Model Uniforms ----------------------- //
uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;
uniform mat3 normal_matrix;
uniform vec3 camera_position;

// -------------------- Vertex IN variables ------------------ //
in vec3 model_view_position;
in vec3 transformed_normal;
in vec2 texture_coord;

// -------------------- OUT variables ------------------ //
out vec4 final_color;


// -------------------- Lighting Functions ------------------- //
vec3 ComputeDirectionalLightColor(DirectionalLight light, vec3 normal, vec3 viewing_direction) {
    // Ambient shading
    vec3 ambient_color = kAmbientStrength * light.color;

    // Diffuse shading
    vec3 direction = normalize(-light.direction);
    float diffuse_value = max(dot(normal, direction), 0.0);
    vec3 diffuse_color = diffuse_value * light.color;

    // Specular shading
    vec3 reflection_direction = reflect(-direction, normal);
    float specular_value = pow(max(dot(viewing_direction, reflection_direction), 0.0),
                               material.specular_exponent);
    vec3 specular_color = specular_value * light.color;

    // Object color
    vec3 texture_color = texture2D(material.diffuse_texture, texture_coord).xyz;
    vec3 object_color = texture_color + material.diffuse_color;

    return light.intensity * object_color * (ambient_color + diffuse_color + specular_color);
}

vec3 ComputePointLightColor(PointLight light, vec3 normal, vec3 viewing_direction) {
    // Ambient shading
    vec3 ambient_color = kAmbientStrength * light.color;

    // Diffuse shading
    vec3 direction = normalize(light.position - model_view_position);
    float diffuse_value = max(dot(normal, direction), 0.0);
    vec3 diffuse_color = diffuse_value * light.color;

    // Specular shading
    vec3 reflection_direction = reflect(-direction, normal);
    float specular_value = pow(max(dot(viewing_direction, reflection_direction), 0.0),
                               material.specular_exponent);
    vec3 specular_color = specular_value * light.color;

    // Attenuation
    float distance = length(light.position - model_view_position);
    float attenuation = 1.0 / (light.attenuation_constant
                                + (light.attenuation_linear * distance)
                                + (light.attenuation_quadratic * distance * distance));

    ambient_color *= attenuation;
    diffuse_color *= attenuation;
    specular_color *= attenuation;

    // Object color
    vec3 texture_color = texture2D(material.diffuse_texture, texture_coord).xyz;
    vec3 object_color = texture_color + material.diffuse_color;

    return object_color * (ambient_color + diffuse_color + specular_color);
}

vec3 ComputeSpotLightColor(SpotLight light, vec3 normal, vec3 viewing_direction) {
    // Ambient shading
    vec3 ambient_color = kAmbientStrength * light.color;

    // Diffuse shading
    vec3 direction = normalize(light.position - model_view_position);
    float diffuse_value = max(dot(normal, direction), 0.0);
    vec3 diffuse_color = diffuse_value * light.color;

    // Specular shading
    vec3 reflection_direction = reflect(-direction, normal);
    float specular_value = pow(max(dot(viewing_direction, reflection_direction), 0.0),
                               material.specular_exponent);
    vec3 specular_color = specular_value * light.color;

    // Spot light intensity
    float theta = dot(direction, normalize(-light.direction));
    float epsilon = (light.inner_cosine - light.outer_cosine);
    float intensity = clamp((theta - light.outer_cosine) / epsilon, 0.0, 1.0);

    diffuse_color *= intensity;
    specular_color *= intensity;

    // Object color
    vec3 texture_color = texture2D(material.diffuse_texture, texture_coord).xyz;
    vec3 object_color = texture_color + material.diffuse_color;

    return object_color * (ambient_color + diffuse_color + specular_color);
}

vec3 ComputeLightingColor(vec3 normal, vec3 viewing_direction) {
    vec3 lighting_color = vec3(0, 0, 0);

    // Compute Directional Light Colors
    for (int i = 0; (i < kMaxDirectionalLightCount) && (i < directional_light_count); ++i)
    {
        lighting_color += ComputeDirectionalLightColor(directional_lights[i], normal, viewing_direction);
    }

    // Compute Point Light Colors
    for (int i = 0; (i < kMaxPointLightCount) && (i < point_light_count); ++i)
    {
        lighting_color += ComputePointLightColor(point_lights[i], normal, viewing_direction);
    }

    // Compute Spot Light Colors
    for (int i = 0; (i < kMaxSpotLightCount) && (i < spot_light_count); ++i)
    {
        lighting_color += ComputeSpotLightColor(spot_lights[i], normal, viewing_direction);
    }

    return lighting_color;
}

// -------------------- Main --------------------------------- //
void main() {
    vec3 normal = normalize(transformed_normal);
    vec3 viewing_direction = normalize(camera_position - model_view_position);
    final_color = vec4(ComputeLightingColor(normal, viewing_direction), 1.0);
}