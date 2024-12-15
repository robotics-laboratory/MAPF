#pragma once

#include "geom/proto/vector.pb.h"

#include <boost/describe.hpp>

#include <ostream>

namespace mapf::geom {

struct Vec2 {
    Vec2();

    Vec2(double x, double y);

    Vec2(proto::Vec2 vec_proto);

    double Len() const;

    double x;
    double y;
};

BOOST_DESCRIBE_STRUCT(Vec2, (), (x, y));

Vec2 operator+(const Vec2& lhs, const Vec2& rhs);

Vec2 operator-(const Vec2& lhs, const Vec2& rhs);

bool Equal(const Vec2& a, const Vec2& b, double eps = 1e-9);

std::ostream& operator<<(std::ostream& out, const Vec2& v);

}  // namespace mapf::geom
