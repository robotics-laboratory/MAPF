#include "geom/vector.h"

#include <utility>

namespace mapf::geom {

Vec2::Vec2() : x{0}, y{0} {}

Vec2::Vec2(double x, double y) : x{x}, y{y} {}

Vec2::Vec2(proto::Vec2 vec_proto) : x{std::move(vec_proto.x())}, y{std::move(vec_proto.y())} {}

double Vec2::Len() const { return hypot(x, y); }

Vec2 operator-(const Vec2& lhs, const Vec2& rhs) { return Vec2(lhs.x - rhs.x, lhs.y - rhs.y); }

}  // namespace mapf::geom