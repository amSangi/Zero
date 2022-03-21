#pragma once

#include "component/Component.hpp"
#include "core/ZeroBase.hpp"

namespace zero
{

	/**
     * @brief Identifies which renderable mesh an Entity is associated with
     */
	struct Mesh : public Component
	{
		/**
         * @brief The name of the model the Entity is associated with
         */
		std::string model_name_;

		/**
         * @brief The unique mesh identifier
         */
		uint32 mesh_id_;

	}; // struct Mesh

} // namespace zero
