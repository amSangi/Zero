#pragma once

#include "entt.hpp"

namespace zero {

	struct Component {
		using Entity = entt::registry<>::entity_type;
		Entity owner_;
	}; // struct Component

} // namespace zero