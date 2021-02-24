#include "core/AssetManager.hpp"
#include <filesystem>

namespace zero
{

constexpr auto kResourcesDirectoryName = "resources";
constexpr auto kShaderDirectoryName = "shaders";
constexpr auto kVertexShaderDirectoryName = "vertex";
constexpr auto kFragmentShaderDirectoryName = "fragment";
constexpr auto kModelDirectoryName = "models";
constexpr auto kTextureDirectoryName = "textures";

void AssetManager::Initialize()
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

    // Cache all vertex shaders in the path <parent_path>/shaders/vertex/...
    // Files in directories under the given path will be cached as (e.g. `<nested directory>/shader_filename.glsl`)
    for (const auto& vertex_shader_entry : std::filesystem::recursive_directory_iterator(vertex_shader_path))
    {
        if (vertex_shader_entry.is_regular_file())
        {
            std::string file_path = vertex_shader_entry.path().string();
            vertex_shader_files_.push_back(file_path.erase(0, vertex_shader_file_path_.size() + 1));
        }
    }
    // Cache all fragment shaders in the path <parent_path>/shaders/fragment/...
    // Files in directories under the given path will be cached as (e.g. `<nested directory>/shader_filename.glsl`)
    for (const auto& fragment_shader_entry : std::filesystem::recursive_directory_iterator(fragment_shader_path))
    {
        if (fragment_shader_entry.is_regular_file())
        {
            std::string file_path = fragment_shader_entry.path().string();
            fragment_shader_files_.push_back(file_path.erase(0, fragment_shader_file_path_.size() + 1));
        }
    }
    // Cache all 3D model files in the path <parent_path>/models/...
    // Files in directories under the given path will be cached as (e.g. `<nested directory>/model_filename.obj`)
    for (const auto& model_entry : std::filesystem::recursive_directory_iterator(model_path))
    {
        if (model_entry.is_regular_file())
        {
            const std::string extension = model_entry.path().extension().string();
            std::string local_file_path = model_entry.path().string();
            local_file_path.erase(0, model_file_path_.size() + 1);
            if (extension == ".obj" || extension == ".fbx")
            {
                model_files_.push_back(local_file_path);
            }
        }
    }
    // Cache all texture files in the path <parent_path>/textures/...
    // Files in directories under the given path will be cached as (e.g. `<nested directory>/texture_filename.png`)
    for (const auto& texture_entry : std::filesystem::recursive_directory_iterator(texture_path))
    {
        if (texture_entry.is_regular_file())
        {
            std::string file_path = texture_entry.path().string();
            texture_files_.push_back(file_path.erase(0, texture_file_path_.size() + 1));
        }
    }
}

const std::vector<std::string>& AssetManager::GetVertexFiles()
{
    return vertex_shader_files_;
}

const std::vector<std::string>& AssetManager::GetFragmentFiles()
{
    return fragment_shader_files_;
}

const std::vector<std::string>& AssetManager::GetTextureFiles()
{
    return texture_files_;
}

const std::vector<std::string>& AssetManager::GetModelFiles()
{
    return model_files_;
}

std::string AssetManager::GetVertexShaderFilePath(const std::string& vertex_shader) const
{
    std::filesystem::path path{vertex_shader_file_path_};
    path.append(vertex_shader);
    return path.string();
}

std::string AssetManager::GetFragmentShaderFilePath(const std::string& fragment_shader) const
{
    std::filesystem::path path{fragment_shader_file_path_};
    path.append(fragment_shader);
    return path.string();
}

std::string AssetManager::GetTextureFilePath(const std::string& texture_file) const
{
    std::filesystem::path path{texture_file_path_};
    path.append(texture_file);
    return path.string();
}

std::string AssetManager::GetModelFilePath(const std::string& model_file) const
{
    std::filesystem::path path{model_file_path_};
    path.append(model_file);
    return path.string();
}

}

