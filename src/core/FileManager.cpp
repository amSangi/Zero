#include "core/FileManager.hpp"
#include <filesystem>

namespace zero
{

constexpr auto kResourcesDirectoryName = "resources";
constexpr auto kShaderDirectoryName = "shaders";
constexpr auto kVertexShaderDirectoryName = "vertex";
constexpr auto kFragmentShaderDirectoryName = "fragment";
constexpr auto kModelDirectoryName = "models";
constexpr auto kTextureDirectoryName = "textures";

void FileManager::Initialize()
{
    std::filesystem::path current_parent_path = std::filesystem::current_path().parent_path();
    const std::filesystem::path resources_path = current_parent_path.append(kResourcesDirectoryName);

    std::filesystem::path shader_path = resources_path;
    shader_path.append(kShaderDirectoryName);

    std::filesystem::path vertex_shader_path = shader_path;
    vertex_shader_path.append(kVertexShaderDirectoryName);
    vertex_shader_file_path_ = vertex_shader_path.string();

    std::filesystem::path fragment_shader_path = shader_path;
    fragment_shader_path.append(kFragmentShaderDirectoryName);
    fragment_shader_file_path_ = fragment_shader_path.string();

    std::filesystem::path model_path = resources_path;
    model_path.append(kModelDirectoryName);
    model_file_path_ = model_path.string();

    std::filesystem::path texture_path = resources_path;
    texture_path.append(kTextureDirectoryName);
    texture_file_path_ = texture_path.string();

    for (const auto& vertex_shader_entry : std::filesystem::recursive_directory_iterator(vertex_shader_path))
    {
        if (vertex_shader_entry.is_regular_file())
        {
            vertex_shader_files_.push_back(vertex_shader_entry.path().filename().string());
        }
    }
    for (const auto& fragment_shader_entry : std::filesystem::recursive_directory_iterator(fragment_shader_path))
    {
        if (fragment_shader_entry.is_regular_file())
        {
            fragment_shader_files_.push_back(fragment_shader_entry.path().filename().string());
        }
    }
    for (const auto& model_entry : std::filesystem::recursive_directory_iterator(model_path))
    {
        if (model_entry.is_regular_file() && model_entry.path().extension().string() == ".obj")
        {
            model_files_.push_back(model_entry.path().filename().string());
        }
    }
    for (const auto& texture_entry : std::filesystem::recursive_directory_iterator(texture_path))
    {
        if (texture_entry.is_regular_file())
        {
            texture_files_.push_back(texture_entry.path().filename().string());
        }
    }
}

const std::vector<std::string>& FileManager::GetVertexFiles()
{
    return vertex_shader_files_;
}

const std::vector<std::string>& FileManager::GetFragmentFiles()
{
    return fragment_shader_files_;
}

const std::vector<std::string>& FileManager::GetTextureFiles()
{
    return texture_files_;
}

const std::vector<std::string>& FileManager::GetModelFiles()
{
    return model_files_;
}

std::string FileManager::GetVertexShaderFilePath(const std::string& vertex_shader) const
{
    std::filesystem::path path{vertex_shader_file_path_};
    path.append(vertex_shader);
    return path.string();
}

std::string FileManager::GetFragmentShaderFilePath(const std::string& fragment_shader) const
{
    std::filesystem::path path{fragment_shader_file_path_};
    path.append(fragment_shader);
    return path.string();
}

std::string FileManager::GetTextureFilePath(const std::string& texture_file) const
{
    std::filesystem::path path{texture_file_path_};
    path.append(texture_file);
    return path.string();
}

std::string FileManager::GetModelFilePath(const std::string& model_file) const
{
    std::filesystem::path path{model_file_path_};
    path.append(model_file);
    return path.string();
}

}

