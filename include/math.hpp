#ifndef METAKMEAN_17_MATH_HPP
#define METAKMEAN_17_MATH_HPP

#include "point.hpp"
#include "types.hpp" // mkm::f64
#include "utility.hpp" // mkm::pun_cast

#include <array>
#include <algorithm> // std::min
#include <tuple> // std::tuple_size_v
#include <utility> // std::pair, std::make_index_sequence

namespace mkm {

static constexpr inline f64 PI_F64 = static_cast<f64>(3.14159265358979324);
static constexpr inline f32 PI_F32 = static_cast<f32>(3.14159265);

constexpr f32 abs(const f32 x) noexcept {
	if (x < 0) {
		return -x;
	}

	return x;
}

constexpr f64 abs(const f64 x) noexcept {
	if (x < 0) {
		return -x;
	}

	return x;
}

constexpr f32 hypot2(const f32 a, const f32 b) noexcept {
	return abs(a) * abs(a) + abs(b) * abs(b);
}

constexpr f64 hypot2(const f64 a, const f64 b) noexcept {
	return abs(a) * abs(a) + abs(b) * abs(b);
}
} // namespace mkm

#endif
