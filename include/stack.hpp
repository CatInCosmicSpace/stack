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
	stack(); // TESTED
	stack(stack const & rhs); // TESTED
	~stack(); 

	auto count() const noexcept -> size_t; // TESTED
	auto top() const -> T; // TESTED
	auto pop() -> void; // TESTED
	auto push(T const & value) -> void; // TESTED

	auto operator=(stack const & rhs) -> stack &; // TESTED
	auto operator==(stack const & rhs) -> bool; // TESTED
	friend ostream & operator<< <>(ostream & output, stack<T> const & stack);
private:
	T * array_;
	size_t array_size_;
	size_t count_;

	auto swap(stack & rhs) -> void;
};

#include "stack.cpp"

#endif // STACK_HPP
