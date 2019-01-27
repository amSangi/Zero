#pragma once

#include "core/ZBase.hpp"

class EntityManager;

namespace zero {

	/**
	 * @brief The Entity in the Entity-Component-System
	 */
	struct Entity {
	public:
		Entity() : index_(0), version_(0) {}
		explicit Entity(uint32 id) : index_(id), version_(id << 24) {}
		Entity(uint32 index, uint32 version) : index_(index), version_(version << 24) {}

		~Entity() = default;
		Entity(const Entity& other) = default;
		Entity& operator=(const Entity& other) = default;

		bool operator==(const Entity& other) const { return index_ == other.index_ && version_ == other.version_; }
		bool operator!=(const Entity& other) const { return !operator==(other);    }
		bool operator<(const Entity& other) const  { return index_ < other.index_; }

		uint32 Index() const                       { return index_; }
		uint32 Version() const                     { return version_; }

		explicit operator bool() const             { return Valid(); }
		bool Valid() const                         { return operator!=(Entity(0)); }

	private:
		uint32 index_   : 24;  // used for indexing
		uint32 version_ : 8;   // used for entity validation

	}; // class EntityId



} // namespace zero