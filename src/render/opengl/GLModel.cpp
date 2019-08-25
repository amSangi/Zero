#include "render/opengl/GLModel.hpp"
#include <assimp/scene.h>

using namespace zero::render;

std::shared_ptr<GLModel> GLModel::CreateGLModel(const aiNode* node, const aiScene* scene) {
    // TODO: Finish Implementation
    return nullptr;
}


GLModel::GLModel(GLuint vao,
                 GLuint vbo,
                 math::Matrix4x4 transformation,
                 Material material,
                 Volume volume,
                 std::shared_ptr<GLModel> parent,
                 std::vector<std::shared_ptr<GLModel>> children)
: vao_(vao)
, vbo_(vbo)
, parent_model_(std::move(parent))
, child_models_(std::move(children))
, transformation_(transformation)
, material_(std::move(material))
, volume_(volume)
{}

GLModel::~GLModel() {
    Cleanup();
}

zero::math::Matrix4x4 GLModel::GetTransformation() const {
    return transformation_;
}

Material GLModel::GetMaterial() const {
    return material_;
}

Volume GLModel::GetVolume() const {
    return volume_;
}

std::shared_ptr<GLModel> GLModel::GetParent() const {
    return parent_model_;
}

const std::vector<std::shared_ptr<GLModel>>& GLModel::GetChildren() const {
    return child_models_;
}

void GLModel::Cleanup() {
    // TODO: Finish Implementation
}