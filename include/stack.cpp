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
	// Создаем пустой битсет, заполненный нулями
	this->reset();
}

inline auto dynamic_bitset::all() const noexcept -> bool {
	// Проврека на то, что все позиции заняты
	auto check = true;
	for (auto i : bits) {
		if (i == false) {
			check = false;
		}
	}
	return check;
}

inline auto dynamic_bitset::any() noexcept -> bool {
	// Проверка на то, что хоть одна позиция занята
	auto check = false;
	for (auto i : bits) {
		if (i == true) {
			check = true;
		}
	}
	return check;
}

inline auto dynamic_bitset::count() const noexcept -> size_t {
	// Количество занятых позиций
	return bits.size();
}

inline auto dynamic_bitset::flip() noexcept -> void {
	// Смена значения всех элементов
	for (auto i : bits) {
		i.flip();
	}
}

inline auto dynamic_bitset::flip(size_t pos) throw(std::out_of_range) -> void {
	// Проверка на обращение по верному индексу
	if (pos < bits.size()) {
		// Смена значения элемента на позиции pos
		bits.at(pos).flip();
	}
	else {
		throw std::out_of_range("out of range");
	}
}

inline auto dynamic_bitset::none() const noexcept -> bool {
	// Ни одна позиция не занята
	auto check = true;
	for (auto i : bits) {
		if (i == true) {
			check = false;
		}
	}
	return check;
}

inline auto dynamic_bitset::reset() noexcept -> void {
	// Установка всех позиций в 0
	for (auto i : bits) {
		i = false;
	}
}

inline auto dynamic_bitset::reset(size_t pos) throw(std::out_of_range) -> void {
	// Проверка на обращение по верному индексу
	if (pos < bits.size()) {
		// Установка элемента на позиции pos в 0
		bits.at(pos) = false;
	}
	else {
		throw std::out_of_range("out of range");
	}
}

inline auto dynamic_bitset::set() noexcept -> void {
	// Установка всех значение в 1
	for (auto i : bits) {
		i = true;
	}
}

inline auto dynamic_bitset::set(size_t pos) throw(std::out_of_range) -> void {
	// Проверка на обращение по верному индексу
	if (pos < bits.size()) {
		// Установка элемента на позиции pos в 0
		bits.at(pos) = true;
	}
	else {
		throw std::out_of_range("out of range");
	}
}

inline auto dynamic_bitset::size() const noexcept -> size_t {
	// Количество элементов, которые можно хранить в битсете
	return bits.capacity();
}

inline auto dynamic_bitset::test(size_t pos) const throw(std::out_of_range) -> bool {
	// Проверка на обращение по верному индексу
	if (pos < bits.size()) {
		// Возвращение бит на позиции pos
		return bits.at(pos);
	}
	else {
		throw std::out_of_range("out of range");
	}
}

inline auto dynamic_bitset::operator[](size_t pos) throw(std::out_of_range) -> bool {
	// Аналогично test
	if (pos < bits.size()) {
		return bits.at(pos);
	}
	else {
		throw std::out_of_range("out of range");
	}
}

inline auto dynamic_bitset::resize() noexcept -> void {
	// Увеличение размера битсета в 2 раза
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
	count_(0),
	bitset_(0) {
}

template<typename T>
inline allocator<T>::allocator(allocator const & other) :
	ptr_(static_cast<T *>(other.count_ == 0 ? nullptr : operator new(other.count_ * sizeof(T)))),
	size_(other.size_),
	bitset_(other.bitset_) {
	for (size_t i = 0; i < other.count_; ++i) {
		this->construct(this->ptr_ + i, other.ptr_[i]);
	}
	this->count_ = other.count_;
}

template<typename T>
inline auto allocator<T>::construct(T * ptr, T const & value) -> void {
	if (ptr < ptr_ || ptr >= ptr_ + size_) {
		throw std::out_of_range("out of range");
	}
	// Создание элемента в аллокаторе
	new(ptr) T(value);
	++count_;
	bitset_.set(ptr - ptr_);
}

template<typename T>
auto allocator<T>::count() const -> size_t {
	return count_;
}

template<typename T> // Удаление одного объекта из аллокатора
auto allocator<T>::destroy(T * ptr) -> void {
	// Проверка на наличие объекта в аллокаторе
	if (ptr < ptr_ || ptr >= ptr_ + size_) {
		throw std::out_of_range("out of range");
	}
	// Явный вызов деструктора
	ptr->~T();
	// Уменьшение кол-ва элементов, хранимых в данный
	// момент в аллокаторе на 1
	--count_;
	// Отмечаем, что данная позиция свободна в битсете
	bitset_.reset(ptr - ptr_);
}

template<typename T>
auto allocator<T>::empty() const -> bool {
	return (count_ == 0);
}

template<typename T>
auto allocator<T>::full() const -> bool {
	return (count_ == size_);
}

template<typename T>
auto allocator<T>::get() -> T * {
	return ptr_;
}

template<typename T>
auto allocator<T>::get() const -> T const * {
	return ptr_;
}

template<typename T>
auto allocator<T>::resize() -> void {
	auto size = size_ * 2 + (size_ == 0);
	T * newArray = copy(ptr_, count_, size);
	delete[] ptr_;
	ptr_ = newArray;
	size_ = size;
	bitset_.resize();
}

template<typename T>
inline allocator<T>::~allocator() {
	// Если в аллокаторе хранятся объекты,
	// то явно вызываем их деструкторы,
	// а затем очищаем память
	if (count_ > 0) {
		destroy(ptr_, ptr_ + count_);
	}
	operator delete(ptr_);
}

template<typename T> /*noexcept*/
auto allocator<T>::swap(allocator & other) -> void {
	std::swap(ptr_, other.ptr_);
	std::swap(count_, other.count_);
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
	alloc(size) {
}

template<typename T> /*strong*/
inline stack<T>::stack(stack const & rhs) :
	alloc(rhs.alloc) {
	/*for (size_t i = alloc.count(); i < rhs.alloc.count(); ++i) {
		alloc.construct(alloc.get() + i, rhs.alloc.get()[i]);
	}*/
}

template<typename T> /*noexcept*/
inline stack<T>::~stack() {
	destroy(alloc.get(), alloc.get() + alloc.count());
}

template<typename T> /*noexcept*/
inline auto stack<T>::count() const noexcept -> size_t {
	return alloc.count();
}

template<typename T> /*noexcept*/
inline auto stack<T>::empty() const noexcept -> bool {
	return (alloc.count() == 0);
}

template<typename T> /*strong*/
inline auto stack<T>::top() const -> const T&{
	if (alloc.count() == 0) {
		throw;
	}
	else {
		return alloc.get()[alloc.count() - 1];
	}
}

template<typename T> /*strong*/
inline auto stack<T>::pop() -> void {
	if (alloc.count() == 0) {
		throw;
	}
	else {
		alloc.destroy(alloc.get() + alloc.count() - 1);
	}
}

template<typename T> /*strong*/
inline auto stack<T>::push(T const & value) -> void {
	if (alloc.empty() == true || alloc.full() == true) {
		alloc.resize();
	}
	alloc.construct(alloc.get() + alloc.count(), value);
}

template<typename T>
auto stack<T>::print() -> void {
	for (size_t i = 0; i < alloc.count(); ++i) {
		std::cout << alloc.get()[i] << " ";
	}
}

template<typename T> /*strong*/
inline auto stack<T>::operator=(stack const & rhs) -> stack & {
	if (this != &rhs) {
		(allocator<T>(rhs.alloc)).swap(this->alloc);
	}
	return *this;
}

template<typename T> /*noexcept*/
inline auto stack<T>::operator==(stack const & rhs) -> bool {
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