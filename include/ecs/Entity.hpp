#pragma once

#include "core/ZBase.hpp"

namespace zero {

	/**
	 * @brief The Entity in the Entity-Component-System
	 */
	class Entity {
	public:
		uint32 index   : 24;  // used for indexing
		uint32 version : 8;   // used for entity validation

		inline bool operator==(const Entity& other) {
			return (index == other.index) && (version == other.version);
		}

		inline bool operator!=(const Entity& other) {
			return !operator==(other);
		}

		inline bool operator<(const Entity& other) {
			return index < other.index;
		}

	}; // class Entity

} // namespace zero