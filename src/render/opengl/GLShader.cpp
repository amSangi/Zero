#include "render/opengl/GLShader.hpp"

namespace zero::render
{

GLShader::GLShader(const std::string& source, IShader::Type type)
: type_(type)
{
    switch (type_)
    {
        case IShader::Type::VERTEX_SHADER:
            id_ = glCreateShader(GL_VERTEX_SHADER);
            break;
        case IShader::Type::FRAGMENT_SHADER:
            id_ = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        case IShader::Type::GEOMETRY_SHADER:
            id_ = glCreateShader(GL_GEOMETRY_SHADER);
            break;
        case IShader::Type::TESSELLATION_CTRL_SHADER:
            id_ = glCreateShader(GL_TESS_CONTROL_SHADER);
            break;
        case IShader::Type::TESSELLATION_EVAL_SHADER:
            id_ = glCreateShader(GL_TESS_EVALUATION_SHADER);
            break;
        case IShader::Type::COMPUTE_SHADER:
            id_ = glCreateShader(GL_COMPUTE_SHADER);
            break;
        default:
            id_ = 0;
            break;
    }

    const GLchar* sources[] = { source.c_str() };
    const GLint lengths[] = { static_cast<GLint>(source.size()) };

    glShaderSource(id_, /* Source Count */ 1, sources, lengths);
}

GLShader::~GLShader()
{
    Cleanup();
}

bool GLShader::Compile()
{
    if (IsCompiled())
    {
        return true;
    }
    glCompileShader(id_);
    return IsCompiled();
}

bool GLShader::IsCompiled() const
{
    GLint is_compiled = GL_FALSE;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &is_compiled);
    return (is_compiled == GL_TRUE);
}

IShader::Type GLShader::GetType() const
{
    return type_;
}

GLuint GLShader::GetNativeIdentifier() const
{
    return id_;
}

void GLShader::Cleanup()
{
    if (glIsShader(id_))
    {
        glDeleteShader(id_);
    }
}

} // namespace zero::render