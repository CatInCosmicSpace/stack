#include "stack.hpp"

#ifndef STACK_CPP
#define STACK_CPP

template<typename T>
inline stack<T>::stack() :	count_(0), 
							array_size_(0) {
}

template<typename T>
inline stack<T>::stack(stack const & rhs) :	array_size_(rhs.array_size_),
											count_(rhs.count_),
											array_(copy(rhs.array_, rhs.count_, rhs.array_size_)) {
	
}

template<typename T>
inline stack<T>::~stack() {
	delete[] array_;
}

template<typename T>
inline auto stack<T>::count() const noexcept -> size_t {
	return count_;
}

template<typename T>
inline auto stack<T>::top() const -> const T {
	if (count_ == 0) {
		throw std::range_error("Empty stack");
	}
	return array_[count_ - 1];
}

template<typename T>
inline auto stack<T>::pop() -> void {
	if (count_ == 0) {
		throw std::range_error("Empty stack");
	}
	--count_;
}

template<typename T>
inline auto stack<T>::push(T const & value) -> void {
	if (count_ == array_size_) {
		auto size = array_size_ * 2 + (array_size_ == 0);
		array_ = copy(array_, count_, array_size_);
		array_size_ = size;
	}
	++count_;
	array_[count_ - 1] = value;
}

template<typename T>
inline auto stack<T>::operator=(stack const & rhs) -> stack & {
	if (this != &rhs) {
		(stack(rhs)).swap(*this);
	}
	return *this;
}

template<typename T>
inline auto stack<T>::operator==(stack const & rhs) noexcept -> bool {
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

template<typename T>
inline auto stack<T>::swap(stack & rhs) noexcept -> void {
	std::swap(rhs.array_size_, array_size_);
	std::swap(rhs.array_, array_);
	std::swap(rhs.count_, count_);
}

template<typename T>
inline auto stack<T>::copy(const T * rhs, size_t sizeLeft, size_t sizeRight) -> T * {
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

//template <typename T>
//ostream & operator<<(ostream & output, stack<T> const & stack) {
//	for (size_t i = 0; i < stack.count_; ++i) {
//		output << stack.array_[i] << " ";
//	}
//	//output << std::endl;
//	return output;
//}
#endif // STACK_CPP