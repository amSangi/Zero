#pragma once

#include "entt.hpp"

namespace zero {

	/**
	 * @brief The Component base in the Entity-Component-System
	 */
	struct Component {
		using Entity = entt::registry<>::entity_type;

		virtual ~Component() = 0;

		Entity owner_;
	}; // struct Component

} // namespace zero