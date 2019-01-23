#pragma once

#include <memory>
#include "Entity.hpp"
#include "EntityManager.hpp"
#include "Registry.hpp"

namespace zero {

	/**
	 * @brief Systems work on entity components and run the game
	 */
	class System {
		friend class Engine;
	public:
		System() = default;
		virtual ~System() = default;

		/**
		 * @brief Initialize the system. Called after construction.
		 */
		virtual void Initialize() = 0;

		/**
		 * @brief Update the system. Called during game loop.
		 * @param dt Time between current and last frame.
		 */
		virtual void Update(float dt) = 0;

		/**
		 * @brief Perform all shutdown operations. Called just before destruction.
		 */
		virtual void ShutDown() = 0;

	protected:


		/* ********** Entity Operations ********** */
		/**
		 * @brief See EntityManager documentation
		 */
		Entity CreateEntity() {
			return entity_manager_->CreateEntity();
		}

		/**
		 * @brief See EntityManager documentation
		 */
		void DestroyEntity(Entity entity) {
			entity_manager_->DestroyEntity(entity);
		}



		/* ********** Component Operations ********** */

		/**
		 * @brief See Registry documentation
		 */
		template<class T>
		T ViewComponent(Entity entity) const {
			return registry_->ViewComponent<T>(entity);
		}

		/**
		 * @brief See Registry documentation
		 */
		template<class T>
		T* ViewAllComponents() const {
			return registry_->ViewAllComponents<T>();
		}

		/**
		 * @brief See Registry documentation
		 */
		template<class T>
		void AddComponent(Entity entity) {
			registry_->AddComponent<T>(entity);
		}

		/**
		 * @brief See Registry documentation
		 */
		template<class T>
		void RemoveComponent(Entity entity) {
			registry_->RemoveComponent<T>(entity);
		}

		/**
		 * @brief See Registry documentation
		 */
		void RemoveAllComponents(Entity entity) {
			registry_->RemoveAllComponents(entity);
		}

		/**
		 * @brief See Registry documentation
		 */
		template<class T>
		void DisableComponent(Entity entity) {
			registry_->DisableComponent<T>(entity);
		}

		/**
		 * @brief See Registry documentation
		 */
		void DisableAllComponents(Entity entity) {
			registry_->DisableAllComponents(entity);
		}

	private:
		std::shared_ptr<EntityManager> entity_manager_;
		std::shared_ptr<Registry> registry_;

	}; // abstract class System

} // namespace zero