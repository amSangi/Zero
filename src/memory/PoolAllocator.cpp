#include "PoolAllocator.hpp"

using namespace zero::memory;

PoolAllocator::PoolAllocator(size_t pool_size) : pool_size_(pool_size) {

}

PoolAllocator::~PoolAllocator() {

}

bool PoolAllocator::operator==(const IAllocator& other) const {
	return false; // stub
}

bool PoolAllocator::operator!=(const IAllocator& other) const {
	return false; // stub
}

void* PoolAllocator::Allocate(size_t n, StorageType type) {

}

void* PoolAllocator::Allocate(size_t n, size_t alignment, size_t offset, StorageType type) {

}

void PoolAllocator::Deallocate(void *pointer, size_t n) {

}