#include <cstdlib>
#include <iostream>
#include <memory>
#ifndef STACK_HPP
#define STACK_HPP

using std::size_t;
using std::ostream;

template<typename T>
class stack {
public:
	stack();
	stack(stack const & rhs);
	~stack();

	auto count() const noexcept->size_t;
	auto top() const->const T&;
	auto pop() -> void;
	auto push(T const & value) -> void;

	auto operator=(stack const & rhs)->stack &;
	auto operator==(stack const & rhs) -> bool;
private:
	T * array_;
	size_t array_size_;
	size_t count_;

	auto copy(const T * rhs, size_t sizeLeft, size_t sizeRight)->T *;
	auto swap(stack & rhs) -> void;
};

#include "stack.cpp"

#endif // STACK_HPP