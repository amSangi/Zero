#pragma once

#include <memory>
#include "render/renderer/Model.hpp"
#include "render/renderer/opengl/GLMesh.hpp"

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
         * @param mesh the mesh data
         * @param transform the Transform prototype
         * @param material the Material prototype
         * @param volume the Volume prototype
         * @param model_instance the ModelInstance prototype
         */
        GLModel(std::shared_ptr<GLMesh> mesh,
                Transform transform,
                Material material,
                Volume volume,
                ModelInstance model_instance);

        ~GLModel() = default;

        std::shared_ptr<IMesh> GetMesh() override;

    private:
        std::shared_ptr<GLMesh> mesh_;

    }; // class GLModel

} // namespace zero::render