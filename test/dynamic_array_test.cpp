#include "dynamic_array.hpp"
#include "point.hpp"
#include "utility.hpp"

#include <iostream>

using namespace mkm::types;

constexpr mkm::Point<> test_array() {
	mkm::DynamicArray<mkm::Point<>, 64> points;

	points.emplace_back(0_f64, 1_f64);
	points.emplace_back(1_f64, 0_f64);
	points.emplace_back(15_f64, 3_f64);

	mkm::swap(points.front(), points.back());

	return points.back();
}


int main() {
	static constexpr auto result = test_array();

	std::cout << result << std::endl;
}
