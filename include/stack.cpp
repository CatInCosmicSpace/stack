#include "stack.hpp"

#ifndef STACK_CPP
#define STACK_CPP

//////////////////////////////
//                          //
//         Bitset           //
//                          //
//////////////////////////////

inline bitset::bitset(size_t size) :
	ptr_(std::make_unique<bool[]>(size)),
	size_(size),
	counter_(0) {
}

inline auto bitset::set(size_t index) throw(std::out_of_range) -> void {
	if (index >= size_) {
		throw (std::out_of_range("out of range"));
	}
	ptr_[index] = true;
	++counter_;
}

inline auto bitset::reset(size_t index) throw(std::out_of_range) -> void {
	if (index >= size_) {
		throw (std::out_of_range("out of range"));
	}
	ptr_[index] = false;
	--counter_;
}

inline auto bitset::test(size_t index) const throw(std::out_of_range) -> bool {
	if (index >= size_) {
		throw (std::out_of_range("out of range"));
	}
	return ptr_[index];
}

inline auto bitset::size() const -> size_t {
	return size_;
}

inline auto bitset::counter() const -> size_t {
	return counter_;
}

//////////////////////////////
//                          //
//        Allocator         //
//                          //
//////////////////////////////

template <typename T>
inline allocator<T>::allocator(size_t size) :
	ptr_(static_cast<T *>(size == 0 ? nullptr : operator new(size * sizeof(T)))), 
	size_(size), 
	map_(std::make_unique<bitset>(size)) {
}

template<typename T>
inline allocator<T>::allocator(allocator const & other) :
	ptr_(static_cast<T *>(other.size_ == 0 ? nullptr : operator new(other.size_ * sizeof(T)))), 
	size_(other.size_), 
	map_(std::make_unique<bitset>(other.size_)) {
	for (size_t i = 0; i < size_; ++i) {
		construct(ptr_ + i, other.ptr_[i]);
	}
}

template <typename T>
inline allocator<T>::~allocator() {
	operator delete(ptr_);
}

template <typename T>
inline auto allocator<T>::resize() -> void {
	auto size = size_ * 2 + (size_ == 0);
	allocator<T> buff(size);
	for (size_t i = 0; i < size_; ++i) {
		buff.construct(buff.ptr_ + i, ptr_[i]);
	}
	this->swap(buff);
	size_ = size;
}

template <typename T>
inline auto allocator<T>::construct(T * ptr, T const & value) -> void {
	if (ptr < ptr_ || ptr >= ptr_ + size_ || map_->test(ptr - ptr_) == true) {
		throw std::out_of_range("out of range");
	}
	new(ptr) T(value);
	map_->set(ptr - ptr_);
}

template <typename T>
inline auto allocator<T>::destroy(T * ptr) -> void {
	if (ptr < ptr_ || ptr >= ptr_ + size_ || map_->test(ptr - ptr_) == false) {
		throw std::out_of_range("out of range");
	}
	ptr->~T();
	map_->reset(ptr - ptr_);
}

template<typename T>
inline auto allocator<T>::get() -> T * {
	return ptr_;
}

template<typename T>
inline auto allocator<T>::get() const -> T const * {
	return ptr_;
}

template<typename T>
inline auto allocator<T>::count() const -> size_t {
	return map_->counter();
}

template<typename T>
inline auto allocator<T>::full() const -> bool {
	return (map_->counter() == size_);
}

template<typename T>
inline auto allocator<T>::empty() const -> bool {
	return (map_->counter() == 0);
}

template <typename T>
inline auto allocator<T>::destroy(T * first, T * last) -> void {
	for (; first != last; ++first) {
		destroy(&*first);
	}
}

template <typename T>
inline auto allocator<T>::swap(allocator & other) -> void {
	std::swap(ptr_, other.ptr_);
	std::swap(size_, other.size_);
	std::swap(map_, other.map_);
}

//////////////////////////////
//                          //
//          Stack           //
//                          //
//////////////////////////////

template <typename T>
inline stack<T>::stack(size_t size) : 
	allocator_(size) {
}

template <typename T>
inline auto stack<T>::operator =(const stack &tmp) -> stack& {
	if (this != &tmp) {
		for (size_t i = 0; i < tmp.count(); i++)
			this->push(*(tmp.allocator_.get() + i));
	}
	return *this;
}

template<typename T>
inline auto stack<T>::empty() const -> bool {
	return (allocator_.count() == 0);
}

template <typename T>
inline auto stack<T>::count() const -> size_t {
	return allocator_.count();
}

template <typename T>
inline auto stack<T>::push(T const &val) -> void {
	if (allocator_.empty() == true || allocator_.full() == true) {
		allocator_.resize();
	}
	allocator_.construct(allocator_.get() + allocator_.count(), val);
}

template <typename T>
inline auto stack<T>::pop() -> void {
	if (allocator_.count() == 0) {
		throw_is_empty();
	}
	allocator_.destroy(allocator_.get() + (this->count() - 1));
}

template <typename T>
inline auto stack<T>::top() -> T& {
	if (allocator_.count() == 0) {
		throw_is_empty();
	}
	return(*(allocator_.get() + this->count() - 1));
}

template <typename T>
inline auto stack<T>::top() const -> const T & {
	if (allocator_.count() == 0) {
		throw_is_empty();
	}
	return(*(allocator_.get() + this->count() - 1));
}

template<typename T>
auto stack<T>::throw_is_empty() throw(std::logic_error) -> void {
	throw (std::range_error("stack is empty"));
}
#endif // STACK_CPP

