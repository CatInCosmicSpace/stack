#include "stack.hpp"

#ifndef STACK_CPP
#define STACK_CPP

template<typename T>
inline stack<T>::stack() : count_(0) {
}

template<typename T>
inline stack<T>::stack(stack const & rhs) :	array_size_(rhs.array_size_),
											count_(rhs.count_),
											array_(new T[rhs.array_size_]) {
	if (count_ > 0) {
		T * m_array = new T[array_size_];
		std::copy(rhs.array_, rhs.array_ + rhs.array_size_, m_array);
		if (array_) {
			delete[] array_;
		}
		array_ = m_array;
	}
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
auto stack<T>::top() const -> T {
	return array_[count_ - 1];
}

template<typename T>
inline auto stack<T>::pop() -> void {
	if (count_ == 0) {
		throw std::logic_error("stack is empty!");
	}
	--count_;
}

template<typename T>
inline auto stack<T>::push(T const & value) -> void {
	if (count_ == array_size_) {
		size_t size = array_size_ * 2 + (array_size_ == 0);
		T * m_array = new T[size];
		std::copy(array_, array_ + array_size_, m_array);
		delete[] array_;
		array_ = m_array;
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

template<typename T>
auto stack<T>::swap(stack & rhs) -> void {
	std::swap(rhs.array_size_, array_size_);
	std::swap(rhs.array_, array_);
	std::swap(rhs.count_, count_);
}

/*template <typename T>
ostream & operator<<(ostream & output, stack<T> const & stack) {
	for (size_t i = 0; i < stack.count_; ++i) {
		output << stack.array_[i] << " ";
	}
	output << std::endl;
	return output;
}*/
#endif // STACK_CPP
