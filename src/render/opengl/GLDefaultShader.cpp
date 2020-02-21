#include "render/opengl/GLDefaultShader.hpp"

using namespace zero::render;

constexpr auto kVertexShaderName = "DefaultVertexShader";
constexpr auto kVertexShaderSource = "#version 450\n"
                                     "precision highp float;\n"
                                     "uniform sampler2D alpha_texture;\n"
                                     "uniform sampler2D ambient_texture;\n"
                                     "uniform sampler2D diffuse_texture;\n"
                                     "uniform sampler2D displacement_texture;\n"
                                     "uniform sampler2D normal_texture;\n"
                                     "uniform mat4 projection_matrix;\n"
                                     "uniform mat4 model_view_matrix;\n"
                                     "layout (location = 0) in vec3 position;\n"
                                     "layout (location = 1) in vec3 normal;\n"
                                     "layout (location = 2) in vec2 texture_coordinate;\n"
                                     "out vec3 model_view_position;\n"
                                     "out vec2 texture_coord;\n"
                                     "void main() {\n"
                                     "    texture_coord = texture_coordinate;\n"
                                     "    \n"
                                     "    vec4 mv_position = model_view_matrix * vec4(position, 1.0);\n"
                                     "    model_view_position = mv_position.xyz;\n"
                                     "    gl_Position = projection_matrix * mv_position;\n"
                                     "}";

constexpr auto kFragmentShaderName = "DefaultFragmentShader";
constexpr auto kFragmentShaderSource = "#version 450\n"
                                       "precision highp float;\n"
                                       "uniform sampler2D alpha_texture;\n"
                                       "uniform sampler2D ambient_texture;\n"
                                       "uniform sampler2D diffuse_texture;\n"
                                       "uniform sampler2D displacement_texture;\n"
                                       "uniform sampler2D normal_texture;\n"
                                       "uniform mat4 projection_matrix;\n"
                                       "uniform mat4 model_view_matrix;\n"
                                       "in vec3 model_view_position;\n"
                                       "in vec2 texture_coord;\n"
                                       "void main() {\n"
                                       "    gl_FragColor = vec4(1, 1, 1, 1);\n"
                                       "}";

ShaderStage GLDefaultShader::kVertexShader = ShaderStage{IShader::Type::VERTEX_SHADER, kVertexShaderName, kVertexShaderSource};
ShaderStage GLDefaultShader::kFragmentShader = ShaderStage{IShader::Type::FRAGMENT_SHADER, kFragmentShaderName, kFragmentShaderSource};
