#include "stack.hpp"
#include <catch.hpp>

SCENARIO("Stack: init", "[init]") {
	stack<size_t> a;
	// object is created and memory allocated for it
	REQUIRE(sizeof(a) != 0);
}

SCENARIO("Stack: operator==", "[op==]") {
	stack<size_t> a, b;
	a.push(1);
	b.push(1);
	REQUIRE(a == b);
}

SCENARIO("Stack: count", "[count]") {
	stack<size_t> a;
	a.push(1);
	a.push(2);
	REQUIRE(a.count() == 2);
}

SCENARIO("Stack: top", "[top]") {
	stack<size_t> a;
	a.push(1);
	a.push(2);
	REQUIRE(a.top() == 2);
}

SCENARIO("Stack: pop", "[pop]") {
	stack<size_t> a;
	a.push(1);
	a.push(2);
	a.pop();
	REQUIRE(a.count() == 1);
}

SCENARIO("Stack: push", "[push]") {
	stack<size_t> a;
	a.push(1);
	REQUIRE(a.top() == 1);
	REQUIRE(a.count() == 1);
}

SCENARIO("Stack: operator=", "[op=]") {
	stack<size_t> a;
	a.push(1);
	stack<size_t> b;
	b = a;
	REQUIRE(b == a);
}

auto push(stack<size_t> st, size_t elem) -> void {
	st.push(elem);
	std::cout << "pushed" << std::endl;
}

auto pop(stack<size_t> st) -> void {
	st.pop();
	std::cout << "popped" << std::endl;
}

SCENARIO("Stack: top and pop threads", "threads") {
	stack<size_t> st;
	st.push(0);
	std::thread t1(&stack<size_t>::push, &st, 1);
	t1.join();
	REQUIRE(st.top() == 1);
	REQUIRE(st.count() == 2);
	REQUIRE(st.empty() == false);
	std::thread t2(&stack<size_t>::pop, &st);
	t2.join();
	REQUIRE(st.top() == 0);
	REQUIRE(st.count() == 1);
	REQUIRE(st.empty() == false);
}
