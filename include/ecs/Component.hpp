#pragma once

#include "Entity.hpp"
#include "ZBase.hpp"

namespace zero {


	/**
	 * @brief The Component in the Entity-Component-System
	 */
	struct Component {
		Entity owner_ = Entity(0);
		uint32 component_id_ = 0;
		bool active_ = true;
	}; // struct Component

} // namespace zero