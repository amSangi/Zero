#pragma once

#include "IAllocator.hpp"
#include "stddef.h"

namespace zero {
namespace memory {

	/**
	 * @brief An Allocator adapter for STL containers
	 * @tparam T the type to allocate
	 */
	template<class T>
	class STLAllocator {
	public:
		/* ********** Member Types ********** */
		using value_type      = T;
		using pointer         = T*;
		using reference       = T&;
		using const_pointer   = const T*;
		using const_reference = const T&;
		using size_type       = size_t;
		using difference_type = ptrdiff_t;

		template<class U>
		struct rebind { typedef STLAllocator<U> other; };

		explicit STLAllocator(const IAllocator& allocator) : allocator_(allocator) {}

		pointer allocate(size_t n, const void* hint = nullptr) { return allocator_.Allocate(n); }
		void deallocate(pointer p, size_t n)                   { allocator_.Deallocate(p, n);}

	private:
		const IAllocator& allocator_;

	}; // template class STLAllocator

} // namespace memory
} // namespace zero