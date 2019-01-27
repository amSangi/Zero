#pragma once

#include <typeinfo>
#include <typeindex>
#include "Entity.hpp"
#include "ZBase.hpp"

namespace zero {


	/**
	 * @brief The Component in the Entity-Component-System
	 */
	template<class T>
	struct Component {
		static const std::type_index kId;
		Entity owner_ = Entity(0);
		bool active_ = true;
	}; // struct Component

	template<class T>
	const std::type_index Component<T>::kId = std::type_index(typeid(T));


} // namespace zero