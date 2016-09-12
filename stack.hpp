#include <cstdlib>
#include <iostream>
#include <memory>
//#include <cassert>
#ifndef STACK_HPP
#define STACK_HPP

using std::size_t;
using std::ostream;

template<typename T>
class stack;

template<typename T>
ostream & operator<<(ostream & output, stack<T> & stack);

template<typename T>
class stack {
public:
	stack();
	stack(stack const & rhs);
	~stack();

	auto count() const noexcept -> size_t;
	auto top() const -> T;
	auto pop() -> void;
	auto push(T const & value) -> void;

	auto operator=(stack const & rhs) -> stack &;
	friend ostream & operator<< <>(ostream & output, stack<T> const & stack);
private:
	T * array_;
	size_t array_size_;
	size_t count_;

	auto swap(stack & rhs) -> void;
};

#include "stack.cpp"

#endif // STACK_HPP
