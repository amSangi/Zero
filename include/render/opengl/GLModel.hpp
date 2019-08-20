#pragma once

#include <memory>
#include <vector>
#include "OpenGL.hpp"
#include "core/ZBase.hpp"
#include "math/Vector3.hpp"
#include "math/Vector2.hpp"
#include "render/IRenderResource.hpp"

namespace zero::render {

    class GLModel : public IRenderResource {
    public:

        GLModel(const std::vector<math::Vec3f>& positions,
                const std::vector<math::Vec3f>& normals,
                const std::vector<math::Vec2f>& texture_coordinates,
                const std::vector<uint32>& indices);

        ~GLModel() override;

    protected:
        void Cleanup() override;

    private:

        /**
         * @brief Vertex Array Object identifier
         */
        GLuint vao_;

        /**
         * @brief Interleaved Vertex Buffer Identifier
         */
        GLuint vbo_;

    }; // class GLModel

} // namespace zero::render