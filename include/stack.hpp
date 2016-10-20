#pragma once 
#include <iostream>
#ifndef STACK_HPP
#define STACK_HPP

using std::size_t;
using std::ostream;

template<typename T>
auto copy(const T * rhs, size_t sizeLeft, size_t sizeRight)->T *; /*strong*/
template <typename T1, typename T2>
auto construct(T1 * ptr, T2 const & value) -> void;
template <typename T>
auto destroy(T * ptr) noexcept -> void;
template <typename FwdIter>
auto destroy(FwdIter first, FwdIter last) noexcept -> void;

template <typename T>
class allocator {
protected:
	explicit allocator(size_t size = 0); /*noexcept*/
	~allocator(); /*noexcept*/
	auto swap(allocator & other) -> void; /*noexcept*/

	allocator(allocator const &) = delete;
	auto operator=(allocator const &)->allocator & = delete;

	T * ptr_;
	size_t size_;
	size_t count_;
};

template<typename T>
class stack : private ::allocator<T> {
public:
	stack(size_t size = 0); /*noexcept*/
	stack(stack const & rhs); /*strong*/
	~stack(); /*noexcept*/

	auto count() const noexcept->size_t; /*noexcept*/
	auto empty() const noexcept -> bool; /*noexcept*/
	auto top() const -> const T&; /*strong*/
	auto pop() -> void; /*strong*/
	auto push(T const & value) -> void; /*strong*/

	auto operator=(stack const & rhs)->stack &; /*strong*/
	auto operator==(stack const & rhs) -> bool; /*noexcept*/
};

#include "stack.cpp"

#endif
