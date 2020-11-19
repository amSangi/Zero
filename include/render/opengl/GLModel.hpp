#pragma once

#include <memory>
#include <vector>
#include "render/Model.hpp"
#include "render/opengl/GLMesh.hpp"

namespace zero::render
{

    /**
     * @brief Renderable OpenGL model
     */
    class GLModel final : public Model
    {
    public:

        /**
         * @brief Constructor. GLModels should be created through CreateGLModel.
         * @param meshes the different meshes that make up the model
         */
        GLModel(std::vector<std::shared_ptr<GLMesh>> meshes,
                Transform transform,
                Material material,
                Volume volume,
                ModelInstance model_instance);

        ~GLModel() = default;

        /**
         * @brief See Model::Draw
         */
        void Draw() override;

    private:

        /**
         * @brief The different meshes that make up the model
         */
        std::vector<std::shared_ptr<GLMesh>> meshes_;

    }; // class GLModel

} // namespace zero::render