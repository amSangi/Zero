#version 450
precision highp float;

// -------------------- Constants ---------------------------- //

// Light Counts
const int kMaxDirectionalLightCount = 8;
const int kMaxPointLightCount = 8;
const int kMaxSpotLightCount = 8;

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
};

uniform Material material;

// -------------------- Model Uniforms ----------------------- //
uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;
uniform vec3 camera_position;

// -------------------- Vertex IN variables ------------------ //
in vec3 model_view_position;
in vec2 texture_coord;

// -------------------- Lighting Functions ------------------- //
vec3 ComputeDirectionalLightIntensity(DirectionalLight light, vec3 position) {
    return light.color;
}

vec3 ComputePointLightColor(PointLight light, vec3 position) {
    float distance = length(light.position - position);
    float attenuation = 1.0 / (light.attenuation_constant
                                + (light.attenuation_linear * distance)
                                + (light.attenuation_quadratic * distance * distance));
    return light.color * attenuation;
}

vec3 ComputeSpotLightColor(SpotLight light, vec3 position) {
    vec3 direction = normalize(light.position - position);
    float theta = dot(direction, normalize(-light.direction));
    float epsilon = (light.inner_cosine - light.outer_cosine);
    float intensity = clamp((theta - light.outer_cosine) / epsilon, 0.0, 1.0);
    return light.color * intensity;
}

// -------------------- Main --------------------------------- //
void main() {
    // Get Material values using texture coordinate
    vec4 diffuse_color = texture2D(material.diffuse_texture, texture_coord);
    vec4 specular_value = texture2D(material.specular_texuture, texture_coord);
    vec4 normal_value = texture2D(material.normal_texture, texture_coord);

    // -------------------- Compute Lighting Color ------------------- //
    vec3 lighting_color = vec3(0, 0, 0);
    // Compute Directional Light Colors
    for (int i = 0; (i < kMaxDirectionalLightCount) && (i < directional_light_count); ++i)
    {
        lighting_color += ComputeDirectionalLightIntensity(directional_lights[i], model_view_position);
    }

    // Compute Point Light Colors
    for (int i = 0; (i < kMaxPointLightCount) && (i < point_light_count); ++i)
    {
        lighting_color += ComputePointLightColor(point_lights[i], model_view_position);
    }

    // Compute Spot Light Colors
    for (int i = 0; (i < kMaxSpotLightCount) && (i < spot_light_count); ++i)
    {
        lighting_color += ComputeSpotLightColor(spot_lights[i], model_view_position);
    }


    // -------------------- Set Color ------------------- //
    gl_FragColor = diffuse_color; //* vec4(lighting_color, 1);
}