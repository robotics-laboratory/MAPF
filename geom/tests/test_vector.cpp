#include "geom/vector.h"
#include "geom/test/test.h"

#include "geom/proto/vector.pb.h"

#include <gtest/gtest.h>

using namespace mapf::geom;

TEST(VectorTest, Constructor1) {
    auto v = Vec2();
    EXPECT_DOUBLE_EQ(v.x, 0);
    EXPECT_DOUBLE_EQ(v.y, 0);
}

TEST(VectorTest, Constructor2) {
    {
        auto v = Vec2(1, 1);
        EXPECT_DOUBLE_EQ(v.x, 1);
        EXPECT_DOUBLE_EQ(v.y, 1);
    }
    {
        auto v = Vec2(1.79851, 6.4719815);
        EXPECT_DOUBLE_EQ(v.x, 1.79851);
        EXPECT_DOUBLE_EQ(v.y, 6.4719815);
    }
}

TEST(VectorTest, ConstructorProto) {
    auto proto_v = proto::Vec2();
    proto_v.set_x(1.79851);
    proto_v.set_y(6.4719815);
    auto v = Vec2(std::move(proto_v));
    EXPECT_DOUBLE_EQ(v.x, 1.79851);
    EXPECT_DOUBLE_EQ(v.y, 6.4719815);
}

TEST(VectorTest, Len) {
    EXPECT_DOUBLE_EQ(Vec2(0, 1).Len(), 1);
    EXPECT_DOUBLE_EQ(Vec2(1.718951, 0).Len(), 1.718951);
    EXPECT_DOUBLE_EQ(Vec2(1, 1).Len(), sqrt(2));
}

TEST(VectorTest, Addition) {
    constexpr double eps = 1e-9;
    ASSERT_GEOM_EQUAL(Vec2(0, 1) + Vec2(0, 0), Vec2(0, 1), eps);
    ASSERT_GEOM_EQUAL(Vec2(1, 1) + Vec2(0, 0), Vec2(1, 1), eps);
    ASSERT_GEOM_EQUAL(
        Vec2(1.718951, 5.1517591) + Vec2(0.51791, 0.16741), Vec2(2.236861, 5.3191691), eps);
}

TEST(VectorTest, Subtraction) {
    constexpr double eps = 1e-9;
    ASSERT_GEOM_EQUAL(Vec2(0, 1) - Vec2(0, 0), Vec2(0, 1), eps);
    ASSERT_GEOM_EQUAL(Vec2(1, 1) - Vec2(0, 0), Vec2(1, 1), eps);
    ASSERT_GEOM_EQUAL(
        Vec2(1.718951, 5.1517591) - Vec2(0.51791, 0.16741), Vec2(1.201041, 4.9843491), eps);
}