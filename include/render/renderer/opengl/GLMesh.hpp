#pragma once

#include "render/IMesh.hpp"
#include "render/renderer/opengl/OpenGL.hpp"

namespace zero::render
{

    class GLMesh : public IMesh
    {
    public:
        GLMesh();

        ~GLMesh() override;
        void Initialize(std::unique_ptr<Mesh> mesh) override;
        const std::vector<std::string>& GetBoneNames() const override;

        GLuint GetVAO() const;
        uint32 GetIndexDataSize() const;
    private:
        GLuint vao_;
        GLuint vertex_vbo_;
        GLuint index_vbo_;
        uint32 index_data_size_;
        std::vector<std::string> bone_names_;

    }; // class GLMesh

} // namespace zero::render