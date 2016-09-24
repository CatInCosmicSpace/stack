#pragma once
#include <iostream>
#ifndef STACK_HPP
#define STACK_HPP

using std::size_t;
using std::ostream;

template<typename T>
auto copy(const T * rhs, size_t sizeLeft, size_t sizeRight)->T *; /*strong*/

template <typename T>
class allocator {
protected:
	explicit allocator(size_t size = 0); /*noexcept*/
	~allocator(); /*noexcept*/
	auto swap(allocator & other) -> void; /*noexcept*/

	allocator(allocator const &) = delete;
	auto operator=(allocator const &) -> allocator & = delete;

	T * ptr_;
	size_t size_;
	size_t count_;
};

template<typename T>
class stack : protected allocator<T> {
public:
	stack(); /*noexcept*/
	stack(stack const & rhs); /*strong*/
	~stack(); /*noexcept*/

	auto count() const noexcept -> size_t; /*noexcept*/
	auto empty() const noexcept -> bool; /*noexcept*/
	auto top() const -> const T&; /*strong*/
	auto pop() -> void; /*strong*/
	auto push(T const & value) -> void; /*strong*/

	auto operator=(stack const & rhs)->stack &; /*strong*/
	auto operator==(stack const & rhs) -> bool; /*noexcept*/
};

#include "stack.cpp"

#endif // STACK_HPP
