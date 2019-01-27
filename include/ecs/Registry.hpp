#pragma once

#include <memory>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include "Entity.hpp"
#include "Component.hpp"

namespace zero {


	/**
	 * @brief A Registry that manages all the components in the game.
	 */
	class Registry {
	public:
		Registry() = default;
		~Registry() = default;

		Registry(const Registry& other) = delete;
		Registry& operator=(const Registry& other) = delete;

		template<class T>
		T* ViewComponent(Entity entity) const {
			// TODO: Finish Implementation
			return nullptr; // stub
		}

		template<class T>
		std::vector<T*> ViewAllComponents() const {
			// TODO: Finish Implementation
			return std::vector<T*>(); // stub
		}

		template<class T>
		void AddComponent(Entity entity) {
			// TODO: Finish Implementation
		}

		void CopyComponents(Entity existing_entity, Entity new_entity) {
			// TODO: Finish Implementation
		}

		template<class T>
		void RemoveComponent(Entity entity) {
			// TODO: Finish Implementation
		}

		void RemoveAllComponents(Entity entity) {
			// TODO: Finish Implementation
		}

		template<class T>
		void DisableComponent(Entity entity) {
			// TODO: Finish Implementation
		}

		void DisableAllComponents(Entity entity) {
			// TODO: Finish Implementation
		}


	private:

	}; // class Registry

} // namespace zero