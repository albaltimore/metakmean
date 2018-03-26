#ifndef METAKMEAN_17_UTILITY_HPP
#define METAKMEAN_17_UTILITY_HPP

#include "types.hpp"

#include <type_traits>
#include <tuple> // std::tuple_size_v, std::get

namespace mkm {

template <typename To, typename From,
          typename = std::enable_if_t<sizeof(To) == sizeof(From)
                                      && alignof(To) == alignof(From)>>
constexpr To& pun_cast(From &from) noexcept {
	return *reinterpret_cast<To*>(&from);
}

template <typename To, typename From,
          typename = std::enable_if_t<sizeof(To) == sizeof(From)
                                      && alignof(To) == alignof(From)>>
constexpr const To& pun_cast(const From &from) noexcept {
	return *reinterpret_cast<const To*>(&from);
}

template <typename T, typename U>
constexpr void swap(T &lhs, U &rhs) noexcept {
	T copy = std::move(lhs);
	lhs = std::move(rhs);
	rhs = std::move(copy);
}

} // namespace std

#endif
