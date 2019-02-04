#pragma once

#include "ZBase.hpp"

namespace zero {
namespace memory {

	/**
	 * @brief Used to indicate allocation lifetime
	 */
	enum StorageType {
		SHORT_TERM,
		LONG_TERM
	};


	/**
	 * @brief An allocator interface
	 * @reference http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2271.html#eastl_allocator
	 */
	class IAllocator {
	public:
		virtual ~IAllocator() = 0;
		virtual void* Allocate(size_t n, StorageType type) = 0;
		virtual void* Allocate(size_t n, size_t alignment, size_t offset, StorageType type) = 0;
		virtual void  Deallocate(void* pointer, size_t n) = 0;
	}; // interface IAllocator


} // namespace memory
} // namespace zero