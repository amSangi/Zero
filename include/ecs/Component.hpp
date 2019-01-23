#pragma once

#include "ZBase.hpp"

namespace zero {

	/**
	 * @brief The Component in the Entity-Component-System
	 */
	class Component {
	public:
		Component() = default;
		virtual ~Component() = default;

		inline void SetActive(bool active) {
			active_ = active;
		}

	protected:
		bool active_;

	}; // abstract class Component

} // namespace zero