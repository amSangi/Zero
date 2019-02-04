#pragma once

#include "IAllocator.hpp"

namespace zero {
namespace memory {

	class PoolAllocator : public IAllocator {
	public:
		explicit PoolAllocator(size_t pool_size);
		~PoolAllocator() override;

		PoolAllocator& operator=(const PoolAllocator& other) = delete;

		bool operator==(const IAllocator& other) const;
		bool operator!=(const IAllocator& other) const;

		void *Allocate(size_t n, StorageType type = SHORT_TERM) override;
		void *Allocate(size_t n, size_t alignment, size_t offset, StorageType type = SHORT_TERM) override;
		void Deallocate(void *pointer, size_t n) override;

	private:
		size_t pool_size_;

		
	}; // class PoolAllocator

} // namespace memory
} // namespace zero