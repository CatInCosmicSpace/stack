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
inline stack<T>::stack() :
	allocator<T>() {
}

template<typename T> /*strong*/
inline stack<T>::stack(stack const & rhs) :
	allocator<T>(rhs.size_) {
	for (size_t i = count_; i < rhs.count_; ++i) {
		construct(ptr_ + i, rhs.ptr_[i]);
	}
	count_ = rhs.count_;
}

template<typename T> /*noexcept*/
inline stack<T>::~stack() {
	destroy(ptr_);
}

template<typename T> /*noexcept*/
inline auto stack<T>::count() const noexcept -> size_t {
	return count_;
}

template<typename T> /*noexcept*/
inline auto stack<T>::empty() const noexcept -> bool {
	return (count_ == 0);
}

template<typename T> /*strong*/
inline auto stack<T>::top() const -> const T&{
	if (count_ == 0) {
		throw std::range_error("stack is empty");
	}
	else {
		return ptr_[count_ - 1];
	}
}

template<typename T> /*strong*/
inline auto stack<T>::pop() -> void {
	if (count_ == 0) {
		throw std::logic_error("stack is empty");
	}
	else {
		destroy(ptr_ + count_ - 1);
		--count_;
	}
}

template<typename T> /*strong*/
inline auto stack<T>::push(T const & value) -> void {
	if (count_ == size_) {
		size_t size = size_ * 2 + (size_ == 0);
		T * newArray = copy(ptr_, count_, size);
		delete[] ptr_;
		ptr_ = newArray;
		size_ = size;
	}
	construct(ptr_ + count_, value);
	//ptr_[count_] = value;
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
inline auto stack<T>::operator==(stack const & rhs) -> bool {
	if ((rhs.count_ != count_) || (rhs.size_ != size_)) {
		return false;
	}
	else {
		for (size_t i = 0; i < count_; i++) {
			if (rhs.ptr_[i] != ptr_[i]) {
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
