#ifndef METAKMEAN_17_POINT_HPP
#define METAKMEAN_17_POINT_HPP

#include "math.hpp"
#include "types.hpp"

#include <iostream>

namespace mkm {

template <typename T = f64,
          typename = std::enable_if_t<std::is_same_v<T, f32>
                                      || std::is_same_v<T, f64>>>
struct Point {
	constexpr Point() = default;

	constexpr Point(const T xv, const T yv) noexcept : x{ xv }, y{ yv } { }

	constexpr Point& operator+=(const Point &other) noexcept {
		x += other.x;
		y += other.y;

		return *this;
	}

	constexpr Point& operator-=(const Point &other) noexcept {
		x -= other.x;
		y -= other.y;

		return *this;
	}

	constexpr T dist2(const Point &other) const noexcept {
		return hypot2(other.x - x, other.y - y);
	}

	constexpr T norm2() const noexcept {
		return hypot2(x, y);
	}

	T x = 0;
	T y = 0;
};

template <typename T>
constexpr Point<T> operator+(const Point<T> &point) noexcept {
	return point;
}

template <typename T>
constexpr Point<T> operator-(const Point<T> &point) noexcept {
	return Point<T>{ -point.x, -point.y };
}

template <typename T>
constexpr Point<T> operator+(const Point<T> &lhs, const Point<T> &rhs) noexcept {
	return Point<T>{ lhs.x + rhs.x, lhs.y + rhs.y };
}

template <typename T>
constexpr Point<T> operator-(const Point<T> &lhs, const Point<T> &rhs) noexcept {
	return Point{ lhs.x - rhs.x, lhs.y - rhs.y };
}

template <typename T>
constexpr bool operator==(const Point<T> &lhs, const Point<T> &rhs) noexcept {
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

template <typename T>
constexpr bool operator!=(const Point<T> &lhs, const Point<T> &rhs) noexcept {
	return !(lhs == rhs);
}

template <typename T>
constexpr bool operator<(const Point<T> &lhs, const Point<T> &rhs) noexcept {
	if (lhs.x == rhs.x) {
		return lhs.y < rhs.y;
	}

	return lhs.x < rhs.x;
}

template <typename T>
constexpr bool operator<=(const Point<T> &lhs, const Point<T> &rhs) noexcept {
	return !(rhs < lhs);
}

template <typename T>
constexpr bool operator>(const Point<T> &lhs, const Point<T> &rhs) noexcept {
	return rhs < lhs;
}

template <typename T>
constexpr bool operator>=(const Point<T> &lhs, const Point<T> &rhs) noexcept {
	return !(lhs < rhs);
}

template <typename T, typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits> &os, const Point<T> &point) {
	return os << '(' << point.x << ", " << point.y << ')';
}

} // namespace mkm

#endif
