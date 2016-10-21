#include "stack.hpp"

#ifndef STACK_CPP
#define STACK_CPP

//////////////////////////////////////////////
//					    //
//		Allocator		    //
//					    //
//////////////////////////////////////////////

template<typename T> /*noexcept*/
inline allocator<T>::allocator(size_t size) :
	ptr_(static_cast<T *>(size == 0 ? nullptr : operator new(size * sizeof(T)))),
	size_(size),
	count_(0) {
}

template<typename T> /*noexcept*/
inline allocator<T>::~allocator() {
	//destroy(ptr_, ptr_ + size_);
	operator delete(ptr_);
}

template<typename T> /*noexcept*/
auto allocator<T>::swap(allocator & other) -> void {
	std::swap(ptr_, other.ptr_);
	std::swap(count_, other.count_);
	std::swap(size_, other.size_);
}

//////////////////////////////////////////////
//				            //
//		Stack			    //
//					    //
//////////////////////////////////////////////

template<typename T> /*noexcept*/
inline stack<T>::stack(size_t size) :
	allocator<T>(size) {
}

template<typename T> /*strong*/
inline stack<T>::stack(stack const & rhs) :
	allocator<T>(rhs.size_) {
	for (size_t i = this->count_; i < rhs.count_; ++i) {
		construct(this->ptr_ + i, rhs.ptr_[i]);
	}
	this->count_ = rhs.count_;
}

template<typename T> /*noexcept*/
inline stack<T>::~stack() {
	destroy(this->ptr_, this->ptr_ + this->count_);
}

template<typename T> /*noexcept*/
inline auto stack<T>::count() const noexcept -> size_t {
	return this->count_;
}

template<typename T> /*noexcept*/
inline auto stack<T>::empty() const noexcept -> bool {
	return (this->count_ == 0);
}

template<typename T> /*strong*/
inline auto stack<T>::top() const throw(std::logic_error) -> const T&{
	if (this->count_ == 0) {
		throw std::logic_error;
	}
	else {
		return this->ptr_[this->count_ - 1];
	}
}

template<typename T> /*strong*/
inline auto stack<T>::pop() throw(std::logic_error) -> void {
	if (this->count_ == 0) {
		throw std::logic_error;
	}
	else {
		destroy(this->ptr_ + this->count_ - 1);
		--this->count_;
	}
}

template<typename T> /*strong*/
inline auto stack<T>::push(T const & value) -> void {
	if (this->count_ == this->size_) {
		size_t array_size = this->size_ * 2 + (this->size_ == 0);

		stack<T> temp{ array_size };
		//stack temp(array_size);
		while (temp.count() < this->count_) {
			temp.push(this->ptr_[temp.count()]);
		}

		this->swap(temp);
	}

	construct(this->ptr_ + this->count_, value);
	++this->count_;
}

template<typename T> /*strong*/
inline auto stack<T>::operator=(stack const & rhs) -> stack & {
	if (this != &rhs) {
		(stack(rhs)).swap(*this);
	}
	return *this;
}

template<typename T> /*noexcept*/
inline auto stack<T>::operator==(stack const & rhs) -> bool {
	if ((rhs.count_ != this->count_) || (rhs.size_ != this->size_)) {
		return false;
	}
	else {
		for (size_t i = 0; i < this->count_; i++) {
			if (rhs.ptr_[i] != this->ptr_[i]) {
				return false;
			}
		}
	}
	return true;
}

template<typename T>  /*strong*/
inline auto copy(const T * rhs, size_t sizeLeft, size_t sizeRight) -> T * {
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

template <typename T1, typename T2>
inline auto construct(T1 * ptr, T2 const & value) -> void {
	new(ptr) T1(value);
}

template <typename T>
inline auto destroy(T * ptr) noexcept -> void {
	ptr->~T();
}

template <typename FwdIter>
inline auto destroy(FwdIter first, FwdIter last) noexcept -> void {
	for (; first != last; ++first) {
		destroy(&*first);
	}
}
#endif // STACK_CPP
