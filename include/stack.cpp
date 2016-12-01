#include "stack.hpp"

#ifndef STACK_CPP
#define STACK_CPP

//////////////////////////////
//                          //
//     Dynamic Bitset       //
//                          //
//////////////////////////////

inline dynamic_bitset::dynamic_bitset(size_t size) noexcept :
bits(size) {
}

inline auto dynamic_bitset::all() const noexcept -> bool {
	auto check = true;
	for (auto i : bits) {
		if (i == false) {
			check = false;
			break;
		}
	}
	return check;
}

inline auto dynamic_bitset::any() noexcept -> bool {
	auto check = false;
	for (auto i : bits) {
		if (i == true) {
			check = true;
			break;
		}
	}
	return check;
}

inline auto dynamic_bitset::count() const noexcept -> size_t {
	size_t counter = 0;
	for (auto i : bits) {
		if (i) {
			++counter;
		}
	}
	return counter;
}

inline auto dynamic_bitset::flip() noexcept -> void {
	for (auto i : bits) {
		i.flip();
	}
}

inline auto dynamic_bitset::flip(size_t pos) throw(std::out_of_range) -> void {
	if (pos < bits.size()) {
		bits.at(pos).flip();
	}
	else {
		throw std::out_of_range("out of range");
	}
}

inline auto dynamic_bitset::none() const noexcept -> bool {
	auto check = true;
	for (auto i : bits) {
		if (i == true) {
			check = false;
			break;
		}
	}
	return check;
}

inline auto dynamic_bitset::reset() noexcept -> void {
	for (auto i : bits) {
		i = false;
	}
}

inline auto dynamic_bitset::reset(size_t pos) throw(std::out_of_range) -> void {
	if (pos < bits.size()) {
		bits.at(pos) = false;
	}
	else {
		throw std::out_of_range("out of range");
	}
}

inline auto dynamic_bitset::set() noexcept -> void {
	for (auto i : bits) {
		i = true;
	}
}

inline auto dynamic_bitset::set(size_t pos) throw(std::out_of_range) -> void {
	if (pos < bits.size()) {
		bits.at(pos) = true;
	}
	else {
		throw std::out_of_range("out of range");
	}
}

inline auto dynamic_bitset::size() const noexcept -> size_t {
	return bits.size();
}

inline auto dynamic_bitset::test(size_t pos) const throw(std::out_of_range) -> bool {
	if (pos < bits.size()) {
		return bits.at(pos);
	}
	else {
		throw std::out_of_range("out of range");
	}
}

inline auto dynamic_bitset::operator[](size_t pos) throw(std::out_of_range) -> bool {
	if (pos < bits.size()) {
		return bits.at(pos);
	}
	else {
		throw std::out_of_range("out of range");
	}
}

inline auto dynamic_bitset::resize() noexcept -> void {
	bits.resize(bits.size() * 2 + (bits.size() == 0));
}

//////////////////////////////
//                          //
//        Allocator         //
//                          //
//////////////////////////////

template<typename T>
inline allocator<T>::allocator(size_t size) :
	ptr_(static_cast<T *>(size == 0 ? nullptr : operator new(size * sizeof(T)))),
	size_(size),
	bitset_(size) {
}

template<typename T>
inline allocator<T>::allocator(allocator const & other) :
	allocator<T>(other.size_) {
	for (size_t i = 0; i < other.bitset_.count(); ++i) {
		if (other.bitset_.test(i) == true) {
			this->construct(this->ptr_ + i, other.ptr_[i]);
		}
	}
}

template<typename T>
inline auto allocator<T>::construct(T * ptr, T const & value) -> void {
	if (ptr < ptr_ || ptr >= ptr_ + size_) {
		throw std::out_of_range("out of range");
	}
	new(ptr) T(value);
	bitset_.set(ptr - ptr_);
}

template<typename T>
inline auto allocator<T>::count() const -> size_t {
	return bitset_.count();
}

template<typename T>
inline auto allocator<T>::destroy(T * ptr) -> void {
	if (ptr < ptr_ || ptr >= ptr_ + size_) {
		throw std::out_of_range("out of range");
	}
	ptr->~T();
	bitset_.reset(ptr - ptr_);
}

template<typename T>
inline auto allocator<T>::empty() const -> bool {
	return (bitset_.count() == 0);
}

template<typename T>
inline auto allocator<T>::full() const -> bool {
	return (bitset_.count() == bitset_.size());
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
inline auto allocator<T>::resize() -> void {
	auto size = size_ * 2 + (size_ == 0);
	allocator<T> buff(size);
	for (size_t i = 0; i < size_; ++i) {
		buff.construct(buff.ptr_ + i, ptr_[i]);
	}
	this->swap(buff);
}

template<typename T>
inline allocator<T>::~allocator() {
	operator delete(ptr_);
}

template<typename T> /*noexcept*/
inline auto allocator<T>::swap(allocator & other) -> void {
	std::swap(ptr_, other.ptr_);
	std::swap(size_, other.size_);
	std::swap(bitset_, other.bitset_);
}

template<typename T>
template<typename FwdIter>
inline auto allocator<T>::destroy(FwdIter first, FwdIter last) noexcept -> void {
	for (; first != last; ++first) {
		destroy(&*first);
	}
}

//////////////////////////////
//                          //
//          Stack           //
//                          //
//////////////////////////////

template<typename T> /*noexcept*/
inline stack<T>::stack(size_t size) :
	alloc(size),
	mut() {
}

template<typename T>
stack<T>::stack(stack const & rhs) :
	alloc(0),
	mut() {
	std::lock_guard<std::mutex> lock(rhs.mut);
	allocator<T>(rhs.alloc).swap(alloc);
}

template<typename T> /*noexcept*/
inline stack<T>::~stack() {
}

template<typename T> /*noexcept*/
inline auto stack<T>::count() const noexcept -> size_t {
	std::lock_guard<std::mutex> lock(mut);
	return alloc.count();
}

template<typename T> /*noexcept*/
inline auto stack<T>::empty() const noexcept -> bool {
	std::lock_guard<std::mutex> lock(mut);
	return (alloc.count() == 0);
}

//template<typename T> /*strong*/
//inline auto stack<T>::top() const -> const T& {
//	std::lock_guard<std::mutex> lock(mut);
//	if (alloc.count() == 0) {
//		throw;
//	}
//	else {
//		const std::shared_ptr<T> top(std::make_shared<T>(alloc.get()[alloc.count() - 1]));
//		return top;
//	}
//}
//
//template<typename T> /*strong*/
//inline auto stack<T>::pop() -> void {
//	std::lock_guard<std::mutex> lock(mut);
//	if (alloc.count() == 0) {
//		throw;
//	}
//	else {
//		alloc.destroy(alloc.get() + alloc.count() - 1);
//	}
//}

template <typename T>
auto stack<T>::pop() throw(std::logic_error) -> const std::shared_ptr<T> {
	std::lock_guard<std::mutex> lock(mut);
	if (alloc.empty()) {
		throw;
	}
	else {
		const std::shared_ptr<T> top(std::make_shared<T>(alloc.get()[alloc.count() - 1]));
		alloc.destroy(alloc.get() + alloc.count() - 1);
		return top;
	}
}

template<typename T> /*strong*/
inline auto stack<T>::push(T const & value) -> void {
	std::lock_guard<std::mutex> lock(mut);
	if (alloc.full() == true) {
		alloc.resize();
	}
	alloc.construct(alloc.get() + alloc.count(), value);
}

template<typename T>
auto stack<T>::print() -> void {
	std::lock_guard<std::mutex> lock(mut);
	for (size_t i = 0; i < alloc.count(); ++i) {
		std::cout << alloc.get()[i] << " ";
	}
}

template<typename T> /*strong*/
inline auto stack<T>::operator=(stack const & rhs) -> stack & {
	if (this != &rhs) {
		std::lock(mut, rhs.mut);
		std::lock_guard<std::mutex> left_lock(mut, std::adopt_lock);
		std::lock_guard<std::mutex> right_lock(rhs.mut, std::adopt_lock);
		(allocator<T>(rhs.alloc)).swap(this->alloc);
	}
	return *this;
}

template<typename T> /*noexcept*/
inline auto stack<T>::operator==(stack const & rhs) -> bool {
	std::lock_guard<std::mutex> lock(mut);
	if (rhs.alloc.count() != this->alloc.count()) {
		return false;
	}
	else {
		for (size_t i = 0; i < alloc.count(); i++) {
			if (this->alloc.get()[i] != rhs.alloc.get()[i]) {
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