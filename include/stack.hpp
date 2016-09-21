#pragma once
#include <cstdlib>
#include <iostream>
#include <memory>
#ifndef STACK_HPP
#define STACK_HPP

using std::size_t;
using std::ostream;
 
template<typename T>
auto copy(const T * rhs, size_t sizeLeft, size_t sizeRight)->T *; /*strong*/

template<typename T>
class stack {
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
private:
	T * array_;
	size_t array_size_;
	size_t count_;

	auto swap(stack & rhs) -> void; /*noexcept*/
};

#include "stack.cpp"

#endif // STACK_HPP
