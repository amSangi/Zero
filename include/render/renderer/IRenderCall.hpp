#pragma once

#include <memory>
#include "render/renderer/IMesh.hpp"
#include "render/renderer/IProgram.hpp"
#include "render/renderer/IRenderHardware.hpp"
#include "render/renderer/ITexture.hpp"
#include "render/renderer/IUniformBuffer.hpp"

namespace zero::render
{
    /**
     * @brief A key used to map render specific data to a RenderCall to aid in RenderCall sorting.
     */
    struct RenderKey
    {
        /**
         * @brief Identifies the render pass.
         * Sort based on the pass. This can be used to ensure shadow passes are performed before a normal render pass,
         * which also ensures shadow map textures are ready for use.
         */
        uint8 pass_;
        /**
         * @brief Unique mesh value that uniquely identifies a mesh.
         * Sort based on mesh ID to minimize vertex state changes.
         */
        uint32 mesh_id_;
        /**
         * @brief Unique material value that uniquely identifies the textures, shaders, and constants (e.g. uniforms).
         * Sort based on material ID to minimize state changes.
         */
        uint32 material_id_;

    }; // struct RenderKey

    /**
     * @brief Interface to a sortable render call
     */
	class IRenderCall
	{
	public:
        /**
         * @brief Retrieve the render key associated with the render call
         * @return the render key
         */
		virtual const RenderKey& GetRenderKey() = 0;
        /**
         * @brief Execute the render call
         * @param rhi the interface to the render hardware used to render
         */
        virtual void Render(std::shared_ptr<IRenderHardware> rhi) = 0;
	}; // class IRenderCall

} // namespace zero::render