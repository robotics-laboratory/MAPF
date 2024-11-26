#pragma once

#include "geom/proto/vector.pb.h"

namespace mapf::geom {

struct Vec2 {
    Vec2();

    Vec2(double x, double y);

    Vec2(proto::Vec2 vec_proto);

    double Len() const;

    double x;
    double y;
};

Vec2 operator-(const Vec2& lhs, const Vec2& rhs);

}  // namespace mapf::geom
