#pragma once

#include <memory>
#include <set>
#include "Entity.hpp"
#include "Registry.hpp"

namespace zero {

	/**
	 * @brief Manages Creation/Destruction of entities.
	 */
	class EntityManager {
		friend class Engine;
	public:
		EntityManager() = default;
		~EntityManager() = default;

		/**
		 * @brief Create a new Entity
		 * @return the new Entity
		 */
		Entity CreateEntity() {
			// TODO: Finish Implementation
			return Entity(); // stub
		}

		/**
		 * @brief Destroy an Entity and all of its components
		 * @param entity the Entity
		 */
		void DestroyEntity(Entity entity) {
			// TODO: Finish Implementation
			registry_->RemoveAllComponents(entity);
		}


	private:
		std::shared_ptr<Registry> registry_;
		std::set<Entity> entities_;

	}; // class EntityManager

} // namespace zero