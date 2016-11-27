#pragma once
#include <iostream>
#include <stdexcept>
#include <vector>
#include <mutex>
#include <thread>

#ifndef STACK_HPP
#define STACK_HPP

using std::size_t;
using std::ostream;

class dynamic_bitset {
public:
	explicit dynamic_bitset(size_t size = 0) noexcept;

	auto all() const noexcept -> bool;
	auto any() noexcept -> bool;
	auto count() const noexcept->size_t;
	auto flip() noexcept -> void;
	auto flip(size_t pos) throw(std::out_of_range) -> void;
	auto none() const noexcept -> bool;
	auto resize() noexcept -> void;
	auto reset() noexcept -> void;
	auto reset(size_t pos) throw(std::out_of_range) -> void;
	auto set() noexcept -> void;
	auto set(size_t pos) throw(std::out_of_range) -> void;
	auto size() const noexcept->size_t;
	auto test(size_t pos) const throw(std::out_of_range) -> bool;

	auto operator[](size_t pos) throw(std::out_of_range) -> bool;
private:
	std::vector<bool> bits;
};

template <typename T>
class allocator {
public:
	explicit allocator(size_t size = 0);
	allocator(allocator const & other);
	auto operator=(allocator const & other)->allocator & = delete;
	~allocator();

	auto construct(T * ptr, T const & value) -> void;
	auto count() const -> size_t;
	auto destroy(T * ptr) -> void;
	auto empty() const -> bool;
	auto full() const -> bool;
	auto get() -> T *;
	auto get() const -> T const *;
	auto resize() -> void;
	auto swap(allocator & other) -> void;
private:
	T * ptr_;
	size_t size_;
	dynamic_bitset bitset_;
	mutable std::mutex mut;

	template <typename FwdIter>
	auto destroy(FwdIter first, FwdIter last) noexcept -> void;
};

template<typename T>
class stack {
public:
	explicit stack(size_t size = 0); /*noexcept*/
	stack(stack const & rhs) = default; /*strong*/
	~stack(); /*noexcept*/

	auto count() const noexcept -> size_t; /*noexcept*/
	auto empty() const noexcept -> bool; /*noexcept*/
	auto top() const -> const T&; /*strong*/
	auto pop() -> void; /*strong*/
	auto push(T const & value) -> void; /*strong*/

	auto print() -> void;

	auto operator=(stack const & rhs) -> stack &; /*strong*/
	auto operator==(stack const & rhs) -> bool; /*noexcept*/
private:
	allocator<T> alloc;

	mutable std::mutex mut;
};

#include "stack.cpp"

#endif