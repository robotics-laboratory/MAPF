#pragma once

#include <gtest/gtest.h>

#include "geom/common.h"

namespace mapf::geom {

template<typename T>
::testing::AssertionResult GeomEqual(const T& lhs, const T& rhs, double eps = 0) {
    if (Equal(lhs, rhs, eps)) {
        return ::testing::AssertionSuccess();
    } else {
        return ::testing::AssertionFailure() << lhs << " != " << rhs << ", eps=" << eps;
    }
}

template<typename T>
::testing::AssertionResult GeomNotEqual(const T& lhs, const T& rhs, double eps = 0) {
    if (!Equal(lhs, rhs, eps)) {
        return ::testing::AssertionSuccess();
    } else {
        return ::testing::AssertionFailure() << lhs << " == " << rhs << ", eps= " << eps;
    }
}

}  // namespace mapf::geom

#define ASSERT_GEOM_EQUAL(...) EXPECT_TRUE(::mapf::geom::GeomEqual(__VA_ARGS__))
#define ASSERT_GEOM_NOT_EQUAL(...) EXPECT_TRUE(::mapf::geom::GeomNotEqual(__VA_ARGS__))