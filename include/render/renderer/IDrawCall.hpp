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
     * @brief A key used to map render specific data to a IDrawCall to aid in IDrawCall sorting.
     */
    struct DrawKey
    {
	    DrawKey()
		: mesh_id_(0)
		, material_id_(0)
		, texture_id_(0)
		{
		}

        /**
         * @brief Unique mesh value that uniquely identifies a mesh.
         * Sort based on mesh ID to minimize vertex state changes.
         */
        uint32 mesh_id_;
        /**
         * @brief Unique material value that identifies the shaders, and constants (e.g. uniforms).
         * Sort based on material ID to minimize state changes.
         */
        uint32 material_id_;
		/**
		 * @brief Unique value that identifies the textures
		 * Sort based on Texture ID to minimize
		 */
		uint32 texture_id_;
    }; // struct DrawKey

    /**
     * @brief Interface to a sortable draw call
     */
	class IDrawCall
	{
	public:

		virtual ~IDrawCall() = default;

        /**
         * @brief Retrieve the draw key associated with the draw call
         * @return the draw key
         */
		virtual const DrawKey& GetDrawKey() = 0;
        /**
         * @brief Execute the draw call
         * @param rhi the interface to the render hardware used to render
         */
        virtual void Draw(IRenderHardware* rhi) = 0;
	}; // class IDrawCall

} // namespace zero::render