#include "render/opengl/GLDefaultShader.hpp"

using namespace zero::render;

constexpr auto kVertexShaderName = "DefaultVertexShader";
constexpr auto kVertexShaderSource = "#version 450\n"
                                     "precision highp float;\n"
                                     "\n"
                                     "uniform mat4 projection_matrix;\n"
                                     "uniform mat4 model_view_matrix;\n"
                                     "uniform mat3 normal_matrix;\n"
                                     "uniform vec3 camera_position;\n"
                                     "\n"
                                     "layout (location = 0) in vec3 position;\n"
                                     "layout (location = 1) in vec3 normal;\n"
                                     "layout (location = 2) in vec2 texture_coordinate;\n"
                                     "\n"
                                     "out vec3 model_view_position;\n"
                                     "out vec3 transformed_normal;\n"
                                     "out vec2 texture_coord;\n"
                                     "\n"
                                     "void main() {\n"
                                     "    texture_coord = texture_coordinate;\n"
                                     "    \n"
                                     "    vec4 mv_position = model_view_matrix * vec4(position, 1.0);\n"
                                     "    model_view_position = mv_position.xyz;\n"
                                     "    transformed_normal = normal_matrix * normal;\n"
                                     "    gl_Position = projection_matrix * mv_position;\n"
                                     "}";

constexpr auto kFragmentShaderName = "DefaultFragmentShader";
constexpr auto kFragmentShaderSource = "#version 450\n"
                                       "precision highp float;\n"
                                       "\n"
                                       "// -------------------- Material Uniforms -------------------- //\n"
                                       "struct Material {\n"
                                       "    sampler2D diffuse_texture;\n"
                                       "    sampler2D specular_texuture;\n"
                                       "    sampler2D normal_texture;\n"
                                       "    vec3 diffuse_color;\n"
                                       "    float specular_exponent;\n"
                                       "};\n"
                                       "\n"
                                       "uniform Material material;\n"
                                       "\n"
                                       "// -------------------- Model Uniforms ----------------------- //\n"
                                       "uniform mat4 projection_matrix;\n"
                                       "uniform mat4 model_view_matrix;\n"
                                       "uniform mat3 normal_matrix;\n"
                                       "uniform vec3 camera_position;\n"
                                       "\n"
                                       "// -------------------- Vertex IN variables ------------------ //\n"
                                       "in vec3 model_view_position;\n"
                                       "in vec3 transformed_normal;\n"
                                       "in vec2 texture_coord;\n"
                                       "\n"
                                       "// -------------------- OUT variables ------------------ //\n"
                                       "out vec4 final_color;\n"
                                       "\n"
                                       "// -------------------- Main --------------------------------- //\n"
                                       "void main() {\n"
                                       "    vec3 texture_color = texture2D(material.diffuse_texture, texture_coord).xyz;\n"
                                       "    vec3 object_color = texture_color + material.diffuse_color;\n"
                                       "    final_color = vec4(object_color, 1.0);\n"
                                       "}";

ShaderStage GLDefaultShader::kVertexShader = ShaderStage{IShader::Type::VERTEX_SHADER, kVertexShaderName, kVertexShaderSource};
ShaderStage GLDefaultShader::kFragmentShader = ShaderStage{IShader::Type::FRAGMENT_SHADER, kFragmentShaderName, kFragmentShaderSource};
