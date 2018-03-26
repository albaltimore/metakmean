#ifndef METAKMEAN_17_DYNAMIC_ARRAY_HPP
#define METAKMEAN_17_DYNAMIC_ARRAY_HPP

#include "types.hpp"

#include <iterator> // std::reverse_iterator
#include <memory> // std::allocator_traits
#include <stdexcept> // std::out_of_range
#include <utility> // std::move, std::swap
#include <initializer_list>

namespace mkm {

template <typename T, usize N,
          typename = std::enable_if_t<std::is_literal_type_v<T>>>
class DynamicArray {
public:
	using value_type = T;
	using size_type = usize;
	using difference_type = isize;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;
	using iterator = T*;
	using const_iterator = const T*;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	constexpr DynamicArray() noexcept : data_{ T{ } } { }

	constexpr DynamicArray(const std::initializer_list<T> ilist) : DynamicArray{ } {
		for (const T &elem : ilist) {
			push_back(elem);
		}
	}

	constexpr reference at(const usize index) {
		if (index > size_) {
			throw std::out_of_range{ "DynamicArray::at" };
		}

		return data_[index];
	}

	constexpr const_reference at(const usize index) const {
		if (index > size_) {
			throw std::out_of_range{ "DynamicArray::at" };
		}

		return data_[index];
	}

	constexpr reference operator[](const usize index) {
		return data_[index];
	}

	constexpr const_reference operator[](const usize index) const {
		return data_[index];
	}

	constexpr reference front() {
		return *begin();
	}

	constexpr const_reference front() const {
		return *cbegin();
	}

	constexpr reference back() {
		return *(end() - 1);
	}

	constexpr const_reference back() const {
		return *(cend() - 1);
	}

	constexpr T* data() noexcept {
		return data_.data();
	}

	constexpr const T* data() const noexcept {
		return data_.data();
	}

	constexpr iterator begin() noexcept {
		return data_.data();
	}

	constexpr const_iterator begin() const noexcept {
		return data_.data();
	}

	constexpr const_iterator cbegin() const noexcept {
		return begin();
	}

	constexpr iterator end() noexcept {
		return data_.data() + size_;
	}

	constexpr const_iterator end() const noexcept {
		return data_.data() + size_;
	}

	constexpr const_iterator cend() const noexcept {
		return end();
	}

	constexpr reverse_iterator rbegin() noexcept {
		return reverse_iterator{ end() };
	}

	constexpr const_reverse_iterator rbegin() const noexcept {
		return const_reverse_iterator{ cend() };
	}

	constexpr const_reverse_iterator crbegin() const noexcept {
		return rbegin();
	}

	constexpr reverse_iterator rend() noexcept {
		return reverse_iterator{ begin() };
	}

	constexpr const_reverse_iterator rend() const noexcept {
		return const_reverse_iterator{ cbegin() };
	}

	constexpr const_reverse_iterator crend() const noexcept {
		return rend();
	}

	constexpr bool empty() const noexcept {
		return size_ == 0;
	}

	constexpr size_type size() const noexcept {
		return size_;
	}

	constexpr size_type capacity() const noexcept {
		return data_.size();
	}

	constexpr void shrink_to_fit() noexcept { }

	constexpr void clear() noexcept {
		size_ = 0;
	}

	constexpr void push_back(const T &x) {
		emplace_back(x);
	}

	constexpr void push_back(T &&x) {
		emplace_back(std::move(x));
	}

	template <typename ...Args,
			  typename = std::enable_if_t<std::is_constructible_v<T, Args...>>>
	constexpr void emplace_back(Args &&...args) {
		data_[size_] = T{ std::forward<Args>(args)... };
		++size_;
	}

	constexpr void pop_back() {
		--size_;
	}

	constexpr void swap(DynamicArray &other) noexcept {
		using std::swap;

		swap(data_, other.data_);
		swap(size_, other.size_);
	}

private:
	std::array<T, N> data_;
	usize size_ = 0;
};

} // namespace mkm

#endif
