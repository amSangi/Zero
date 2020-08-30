#pragma once

#include <vector>
#include <string>

namespace zero
{

    /**
     * @brief Manage the file path and file names of different resource files
     */
    class FileManager
    {
    public:
        FileManager() = default;
        ~FileManager() = default;

        /**
         * @brief Retrieve all the files and their fully qualified file paths
         */
        void Initialize();

        /**
         * @brief Get the list of file names for a resource
         */
        ///@{
        const std::vector<std::string>& GetVertexFiles();
        const std::vector<std::string>& GetFragmentFiles();
        const std::vector<std::string>& GetTextureFiles();
        const std::vector<std::string>& GetModelFiles();
        ///@}

        /**
         * @brief Get the fully qualified file path of a resource
         */
        ///@{
        [[nodiscard]] std::string GetVertexShaderFilePath(const std::string& vertex_shader) const;
        [[nodiscard]] std::string GetFragmentShaderFilePath(const std::string& fragment_shader) const;
        [[nodiscard]] std::string GetTextureFilePath(const std::string& texture_file) const;
        [[nodiscard]] std::string GetModelFilePath(const std::string& model_file) const;
        ///@}

    private:
        std::string vertex_shader_file_path_;
        std::string fragment_shader_file_path_;
        std::string texture_file_path_;
        std::string model_file_path_;
        std::vector<std::string> vertex_shader_files_;
        std::vector<std::string> fragment_shader_files_;
        std::vector<std::string> texture_files_;
        std::vector<std::string> model_files_;
    }; // class FileManager

} // namespace zero