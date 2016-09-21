#include "stack.hpp"

#ifndef STACK_CPP
#define STACK_CPP

template<typename T> /*noexcept*/
inline stack<T>::stack() :	count_(0),
							array_size_(0),
							array_(nullptr) {
}

template<typename T> /*strong*/
inline stack<T>::stack(stack const & rhs) : 
	array_size_(rhs.array_size_),
	count_(rhs.count_),
	array_(copy(rhs.array_, rhs.count_, rhs.array_size_)) {
}

template<typename T> /*noexcept*/
inline stack<T>::~stack() {
	delete[] array_;
}

template<typename T> /*noexcept*/
inline auto stack<T>::count() const noexcept -> size_t {
	return count_;
}

template<typename T> /*noexcept*/
auto stack<T>::empty() const noexcept -> bool {
	return (count_ == 0);
}

template<typename T> /*strong*/
auto stack<T>::top() const -> const T& {
	if (count_ == 0) {
		throw std::range_error("stack is empty");
	}
	else {
		return array_[count_ - 1];
	}
}

template<typename T> /*strong*/
inline auto stack<T>::pop() -> void {
	if (count_ == 0) {
		throw std::logic_error("stack is empty");
	}
	else {
		--count_;
	}
}

template<typename T> /*strong*/
inline auto stack<T>::push(T const & value) -> void {
	if (count_ == array_size_) {
		size_t size = array_size_ * 2 + (array_size_ == 0);
		T * newArray = copy(array_, count_, size);
		delete[] array_;
		array_ = newArray;
		array_size_ = size;
	}
	array_[count_] = value;
	++count_;
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
	if ((rhs.count_ != count_) || (rhs.array_size_ != array_size_)) {
		return false;
	}
	else {
		for (size_t i = 0; i < count_; i++) {
			if (rhs.array_[i] != array_[i]) {
				return false;
			}
		}
	}
	return true;
}

template<typename T> /*noexcept*/
auto stack<T>::swap(stack & rhs) -> void {
	std::swap(rhs.array_size_, array_size_);
	std::swap(rhs.array_, array_);
	std::swap(rhs.count_, count_);
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