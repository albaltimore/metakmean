#include "dynamic_array.hpp"
#include "point.hpp"
#include "types.hpp"

#include <array>
#include <limits>
#include <algorithm>

using namespace mkm::types;

template <typename T, usize N>
class KMeans {
public:
	constexpr KMeans(const std::initializer_list<mkm::Point<T>> ilist) noexcept
	: data_{ ilist }, clusters_{ mkm::DynamicArray<mkm::Point<T>, 512>{ } },
	  means_{ { mkm::Point<T>{ } } } {
		calculate_means();
	}

	template <usize I>
	constexpr const mkm::Point<T>& get() const noexcept {
		static_assert(I < N);

		return means_[I];
	}

private:
	constexpr void calculate_means() {
		initialize_means();

		for (usize i = 0; i < 64; ++i) {
			assign_clusters();
			update_means();
		}
	}

	// take the min and maximum x, y values and linearly interpolate between
	// them
	constexpr void initialize_means() {
		mkm::Point<T> min = data_.front();
		mkm::Point<T> max = data_.front();

		for (const auto &point : data_) {
			min.x = std::min(min.x, point.x);
			min.y = std::min(min.y, point.y);

			max.x = std::max(max.x, point.x);
			max.y = std::max(max.y, point.y);
		}

		const T x_range = max.x - min.x;
		const T y_range = max.y - min.y;

		const T x_step = x_range / static_cast<T>(N);
		const T y_step = y_range / static_cast<T>(N);

		T x = min.x;
		T y = min.y;

		for (auto &mean : means_) {
			mean = mkm::Point<T>(x, y);
			x += x_step;
			y += y_step;
		}
	}

	constexpr void assign_clusters() {
		for (auto &cluster : clusters_) {
			cluster.clear();
		}

		for (const auto &point : data_) {
			usize min_index = std::numeric_limits<usize>::max();
			f64 min_distance = std::numeric_limits<f64>::max();

			for (usize i = 0; i < N; ++i) {
				if (point.dist2(means_[i]) < min_distance) {
					min_distance = point.dist2(means_[i]);
					min_index = i;
				}
			}

			clusters_[min_index].push_back(point);
		}
	}

	constexpr void update_means() {
		for (usize i = 0; i < N; ++i) {
			means_[i] = mkm::Point<T>{ };

			for (const auto &point : clusters_[i]) {
				means_[i] += point;
			}

			means_[i].x /= static_cast<f64>(clusters_[i].size());
			means_[i].y /= static_cast<f64>(clusters_[i].size());
		}
	}

	mkm::DynamicArray<mkm::Point<T>, 512> data_;
	std::array<mkm::DynamicArray<mkm::Point<T>, 512>, N> clusters_;
	std::array<mkm::Point<T>, N> means_;
};

int main() {
	constexpr KMeans<f64, 3> means{
	{ 0.722440, 0.149865 }, { 0.659605, 0.518595 }, { 0.972975, 0.648991 },
	{ 0.800331, 0.453798 }, { 0.432392, 0.825314 }, { 0.083470, 0.133171 },
	{ 0.173389, 0.390938 }, { 0.831380, 0.803364 }, { 0.060471, 0.399258 },
	{ 0.526876, 0.416799 }, { 0.656860, 0.627973 }, { 0.291984, 0.431651 },
	{ 0.015487, 0.984064 }, { 0.167168, 0.106216 }, { 0.372410, 0.198118 },
	{ 0.489688, 0.339493 }, { 0.951630, 0.920332 }, { 0.052677, 0.737858 },
	{ 0.269119, 0.422836 }, { 0.547871, 0.942737 }, { 0.417744, 0.983052 },
	{ 0.301455, 0.701099 }, { 0.666339, 0.539126 }, { 0.698106, 0.666528 },
	{ 0.178132, 0.128014 }, { 0.999080, 0.171121 }, { 0.032601, 0.561200 },
	{ 0.881867, 0.669175 }, { 0.190433, 0.368917 }, { 0.460726, 0.981638 },
	{ 0.156405, 0.855523 }, { 0.644765, 0.376272 }, { 0.190924, 0.428253 },
	{ 0.482022, 0.120612 }, { 0.589507, 0.226188 }, { 0.384619, 0.582986 },
	{ 0.251806, 0.290441 }, { 0.617091, 0.265281 }, { 0.824376, 0.982663 },
	{ 0.730249, 0.343877 }, { 0.584069, 0.107769 }, { 0.906308, 0.879654 },
	{ 0.817761, 0.260728 }, { 0.594356, 0.022513 }, { 0.425259, 0.312719 },
	{ 0.161485, 0.178766 }, { 0.422886, 0.094229 }, { 0.598524, 0.470924 },
	{ 0.695949, 0.699888 }, { 0.638531, 0.033604 }, { 0.068806, 0.319600 },
	{ 0.530864, 0.654446 }, { 0.407619, 0.819981 }, { 0.718359, 0.968649 },
	{ 0.531334, 0.325146 }, { 0.105629, 0.610959 }, { 0.778802, 0.423453 },
	{ 0.090823, 0.266471 }, { 0.153657, 0.281005 }, { 0.440085, 0.527143 },
	{ 0.457424, 0.875372 }, { 0.518052, 0.943623 }, { 0.637709, 0.957694 },
	{ 0.240707, 0.676122 }, { 0.289065, 0.671808 }, { 0.695140, 0.067993 },
	{ 0.254790, 0.224040 }, { 0.667833, 0.844392 }, { 0.344462, 0.780520 },
	{ 0.675332, 0.006715 }, { 0.602170, 0.386771 }, { 0.915991, 0.001151 },
	{ 0.462449, 0.424349 }, { 0.460916, 0.770160 }, { 0.322472, 0.784739 },
	{ 0.471357, 0.035763 }, { 0.175874, 0.721758 }, { 0.473486, 0.152721 },
	{ 0.341125, 0.607389 }, { 0.191745, 0.738427 }, { 0.242850, 0.917424 },
	{ 0.269062, 0.765500 }, { 0.188662, 0.287498 }, { 0.091113, 0.576209 },
	{ 0.683363, 0.546593 }, { 0.425729, 0.644443 }, { 0.647618, 0.679017 },
	{ 0.635787, 0.945174 }, { 0.208935, 0.709282 }, { 0.236231, 0.119396 },
	{ 0.607304, 0.450138 }, { 0.458725, 0.661945 }, { 0.770286, 0.350218 },
	{ 0.662010, 0.416159 }, { 0.841929, 0.832917 }, { 0.256441, 0.613461 },
	{ 0.582249, 0.540739 }, { 0.869941, 0.264779 }, { 0.318074, 0.119215 },
	{ 0.939829, 0.645552 }, { 0.479463, 0.639317 }, { 0.544716, 0.647311 },
	{ 0.543886, 0.721047 }, { 0.522495, 0.993705 }, { 0.218677, 0.105798 },
	{ 0.109697, 0.063591 }, { 0.404580, 0.448373 }, { 0.365816, 0.763505 },
	{ 0.627896, 0.771980 }, { 0.932854, 0.972741 }, { 0.192028, 0.138874 },
	{ 0.696266, 0.093820 }, { 0.525404, 0.530344 }, { 0.861140, 0.484853 },
	{ 0.393456, 0.671431 }, { 0.741258, 0.520052 }, { 0.347713, 0.149997 },
	{ 0.586092, 0.262145 }, { 0.044454, 0.754933 }, { 0.242785, 0.442402 },
	{ 0.687796, 0.359228 }, { 0.736340, 0.394707 }, { 0.683416, 0.704047 },
	{ 0.442305, 0.019578 }, { 0.330858, 0.424309 }, { 0.270270, 0.197054 },
	{ 0.821721, 0.429921 }, { 0.887771, 0.391183 }
	};

	std::cout << "means = [" << means.get<0>() << ", " << means.get<1>()
	          << ", " << means.get<2>() << "]" << std::endl;
}
