#pragma once

#include <vector>
#include <string>

namespace zero
{

    class FileManager
    {
    public:
        FileManager() = default;
        ~FileManager() = default;

        void Initialize();

        const std::vector<std::string>& GetVertexFiles();
        const std::vector<std::string>& GetFragmentFiles();
        const std::vector<std::string>& GetTextureFiles();
        const std::vector<std::string>& GetModelFiles();

        [[nodiscard]] std::string GetVertexShaderFilePath(const std::string& vertex_shader) const;
        [[nodiscard]] std::string GetFragmentShaderFilePath(const std::string& fragment_shader) const;
        [[nodiscard]] std::string GetTextureFilePath(const std::string& texture_file) const;
        [[nodiscard]] std::string GetModelFilePath(const std::string& model_file) const;

    private:
        std::string vertex_shader_file_path_;
        std::string fragment_shader_file_path_;
        std::string texture_file_path_;
        std::string model_file_path_;
        std::vector<std::string> vertex_shader_files_;
        std::vector<std::string> fragment_shader_files_;
        std::vector<std::string> texture_files_;
        std::vector<std::string> model_files_;
    };

} // namespace zero