#pragma once

#include <cmath>

namespace mapf::geom {

inline bool Equal(double a, double b, double eps = 1e-9) noexcept { return std::abs(a - b) <= eps; }

}  // namespace mapf::geom