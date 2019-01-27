#pragma once

#include <iterator>
#include <vector>
#include "Entity.hpp"
#include "Registry.hpp"

namespace zero {

	/**
	 * @brief Manages Creation/Destruction of entities.
	 */
	class EntityManager {
		friend class Engine;
	public:

		/* ********** Iterator ********** */

		/**
		 * @brief Iterator for all valid entities the EntityManager is managing
		 * @tparam T the vector<Entity> iterator type
		 */
		template <class T>
		class EntityIterator : public std::iterator<std::input_iterator_tag, Entity> {
		public:
			EntityIterator(const EntityManager& entity_manager, T it, T end)
				           : entity_manager_(entity_manager), it_(it), end_(end) {}

			bool operator==(const EntityIterator& other) const { return it_ == other.it_;   }
			bool operator!=(const EntityIterator& other) const { return !operator==(other); }

			Entity& operator*() { return *it_; }
			const Entity& operator*() const { return *it_; }

			Entity& operator++() {
				++it_;
				while (Predicate() && it_ != end_) {
					++it_;
				}
				return operator*();
			}

		protected:
			bool Predicate() const {
				return entity_manager_.IsValidEntity(*it_);
			}

		private:
			T it_, end_;
			const EntityManager& entity_manager_;

		}; // template class EntityIterator

		using Iterator = EntityIterator<std::vector<Entity>::iterator>;
		using ConstIterator = EntityIterator<std::vector<Entity>::const_iterator>;

		/* ****************************** */

		EntityManager() = default;
		explicit EntityManager(uint64 capacity) : entities_(capacity) {}
		~EntityManager() = default;

		EntityManager(const EntityManager& other) = delete;
		EntityManager& operator=(const EntityManager& other) = delete;


		/* ********** Getters ********** */

		/**
		 * @brief Return the number of managed entities
		 * @return the number of managed entities
		 */
		uint64 EntityCount() const      { return entities_.size() - available_entities_.size();}

		/**
		 * @brief Return the number of entities that can be held
		 * @return the entities capacity
		 */
		uint64 EntityCapacity() const   { return entities_.capacity(); }

		ConstIterator CBegin() const { return ConstIterator(*this, entities_.cbegin(), entities_.cend()); }
		ConstIterator CEnd() const   { return ConstIterator(*this, entities_.cend(), entities_.cend());   }
		Iterator Begin()             { return Iterator(*this, entities_.begin(), entities_.end()); }
		Iterator End()               { return Iterator(*this, entities_.end(), entities_.end());   }


		/* ********** Entity Operations ********** */

		/**
		 * @brief Create a new entity
		 * @return the new entity
		 */
		Entity CreateEntity() {
			uint32 index, version;
			if (available_entities_.empty()) {
				index = ++index_counter_;
				UpdateCapacity(index);
				version = 1;
			}
			else {
				Entity available_entity = available_entities_.back();
				index = available_entity.Index();
				version = available_entity.Version() + 1;
				available_entities_.pop_back();
			}
			Entity entity(index, version);
			entities_[index] = entity;
			return entity;
		}

		/**
		 * @brief Create a new entity and copy the components of an existing entity
		 * @param existing_entity the existing entity
		 * @return the new entity
		 */
		Entity CopyEntity(Entity existing_entity) {
			Entity new_entity = CreateEntity();
			registry_->CopyComponents(existing_entity, new_entity);
			return new_entity;
		}

		/**
		 * @brief Destroy an entity and its components
		 * @param entity the entity to destroy
		 */
		void DestroyEntity(Entity entity) {
			entities_[entity.Index()] = Entity(0);
			available_entities_.push_back(entity);
			registry_->RemoveAllComponents(entity);
		}

		/**
		 * @brief Check to see if an entity is valid
		 * @param entity the entity to check
		 * @return True if the entity is valid. Otherwise false.
		 */
		bool IsValidEntity(Entity entity) const {
			return (entity.Index() < entities_.size()) && (entities_[entity.Index()].Version() == entity.Version());
		}

	private:
		static const uint8 kResizeFactor = 2;

		uint32 index_counter_ = 0;
		std::shared_ptr<Registry> registry_;
		std::vector<Entity> entities_;
		std::vector<Entity> available_entities_;


		/**
		 * @brief Updates the entity capacity
		 * @param index the new index to make room for
		 */
		void UpdateCapacity(uint32 index) {
			if (index >= entities_.size()) {
				entities_.resize(index * kResizeFactor);
			}
		}

	}; // class EntityManager

} // namespace zero