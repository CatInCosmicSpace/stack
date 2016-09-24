#include "stack.hpp"

#ifndef STACK_CPP
#define STACK_CPP

//////////////////////////////////////////////
//										//
//				  Allocator				//
//										//
//////////////////////////////////////////////

template<typename T> /*noexcept*/
inline allocator<T>::allocator(size_t size) :
	ptr_(static_cast<T *>(size == 0 ? nullptr : operator new(size * sizeof(T)))),
	size_(0),
	count_(size) {
}

template<typename T> /*noexcept*/
inline allocator<T>::~allocator() {
	delete[] ptr_;
}

template<typename T> /*noexcept*/
auto allocator<T>::swap(allocator & other) -> void {
	std::swap(ptr_, other.ptr_);
	std::swap(count_, other.count_);
	std::swap(size_, other.size_);
}

//////////////////////////////////////////////
//										//
//				  Stack					//
//										//
//////////////////////////////////////////////

template<typename T> /*noexcept*/
inline stack<T>::stack() :
	allocator<T>() {
}

template<typename T> /*strong*/
inline stack<T>::stack(stack const & rhs) :
	allocator<T>(rhs.count_) {
	allocator<T>::ptr_ = copy(rhs.ptr_, rhs.count_, rhs.size_);
	allocator<T>::size_ = rhs.size_;
}

template<typename T> /*noexcept*/
inline stack<T>::~stack() {
}

template<typename T> /*noexcept*/
inline auto stack<T>::count() const noexcept -> size_t {
	return allocator<T>::count_;
}

template<typename T> /*noexcept*/
auto stack<T>::empty() const noexcept -> bool {
	return (allocator<T>::count_ == 0);
}

template<typename T> /*strong*/
auto stack<T>::top() const -> const T& {
	if (allocator<T>::count_ == 0) {
		throw std::range_error("stack is empty");
	}
	else {
		return allocator<T>::ptr_[allocator<T>::count_ - 1];
	}
}

template<typename T> /*strong*/
inline auto stack<T>::pop() -> void {
	if (allocator<T>::count_ == 0) {
		throw std::logic_error("stack is empty");
	}
	else {
		--allocator<T>::count_;
	}
}

template<typename T> /*strong*/
inline auto stack<T>::push(T const & value) -> void {
	if (allocator<T>::count_ == allocator<T>::size_) {
		size_t size = allocator<T>::size_ * 2 + (allocator<T>::size_ == 0);
		T * newArray = copy(allocator<T>::ptr_, allocator<T>::count_, size);
		delete[] allocator<T>::ptr_;
		allocator<T>::ptr_ = newArray;
		allocator<T>::size_ = size;
	}
	allocator<T>::ptr_[allocator<T>::count_] = value;
	++allocator<T>::count_;
}

template<typename T> /*strong*/
inline auto stack<T>::operator=(stack const & rhs) -> stack & {
	if (this != &rhs) {
		(stack(rhs)).swap(*this);
	}
	return *this;
}

template<typename T> /*noexcept*/
auto stack<T>::operator==(stack const & rhs) -> bool {
	if ((rhs.count_ != allocator<T>::count_) || (rhs.size_ != allocator<T>::size_)) {
		return false;
	}
	else {
		for (size_t i = 0; i < allocator<T>::count_; i++) {
			if (rhs.ptr_[i] != allocator<T>::ptr_[i]) {
				return false;
			}
		}
	}
	return true;
}

template<typename T>  /*strong*/
auto copy(const T * rhs, size_t sizeLeft, size_t sizeRight) -> T * {
	T * m_array = new T[sizeRight];
	try {
		std::copy(rhs, rhs + sizeLeft, m_array);
	}
	catch (...) {
		delete[] m_array;
		throw;
	}
	return m_array;
}

#endif // STACK_CPP
