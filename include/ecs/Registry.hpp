#pragma once

#include "Entity.hpp"

namespace zero {

	/**
	 * @brief A Registry that manages all the components in the game.
	 */
	class Registry {
	public:
		Registry() = default;
		~Registry() = default;



		/**
		 * @brief Retrieve component T that belongs to the entity
		 * @tparam T the component type
		 * @param entity the entity
		 * @return the component of type T of the entity
		 */
		template<class T>
		T ViewComponent(Entity entity) const {
			// TODO: Finish Implementation
			return T();
		}



		/**
		 * @brief Retrieve all the components of a given type
		 * @tparam T the component type
		 * @return all the components with the given type
		 */
		template<class T>
		T* ViewAllComponents() const {
			// TODO: Finish Implementation
			return nullptr;
		}



		/**
		 * @brief Add a new component of type T to the entity
		 * @tparam T the component type
		 * @param entity the entity
		 */
		template<class T>
		void AddComponent(Entity entity) {
			// TODO: Finish Implementation
		}



		/**
		 * @brief Remove component T that belongs to the entity
		 * @tparam T the component type
		 * @param entity the entity
		 */
		template<class T>
		void RemoveComponent(Entity entity) {
			// TODO: Finish Implementation
		}



		/**
		 * @brief Remove all components that belong to the entity
		 * @param entity the entity
		 */
		void RemoveAllComponents(Entity entity) {
			// TODO: Finish Implementation
		}



		/**
		 * @brief Disable component T of the entity
		 * @tparam T the component type
		 * @param entity the entity
		 */
		template<class T>
		void DisableComponent(Entity entity) {
			// TODO: Finish Implementation
		}



		/**
		 * @brief Disable all components that belong to the entity
		 * @param entity the entity
		 */
		void DisableAllComponents(Entity entity) {
			// TODO: Finish Implementation
		}


	private:
		// TODO: Create Component Containers

	}; // class Registry

} // namespace zero