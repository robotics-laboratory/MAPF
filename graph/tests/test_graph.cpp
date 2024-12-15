#include "graph/graph.h"
#include "graph/test/test.h"

#include <gtest/gtest.h>

using namespace mapf::graph;

TEST(GraphTest, ValidPath) {
    auto nodes = Nodes{{0, Node(0)}, {1, Node(1)}, {2, Node(2)}, {3, Node(3)}};
    auto edges = Edges{Edge(0, 1), Edge(1, 3), Edge(3, 2), Edge(2, 0)};
    auto graph = Graph(std::move(nodes), std::move(edges));
    {
        auto path = NodeIdsList{};
        ASSERT_VALID_PATH(graph, path);
    }
    {
        auto path = NodeIdsList{0};
        ASSERT_VALID_PATH(graph, path);
    }
    {
        auto path = NodeIdsList{0, 1};
        ASSERT_VALID_PATH(graph, path);
    }
    {
        auto path = NodeIdsList{0, 1, 3};
        ASSERT_VALID_PATH(graph, path);
    }
    {
        auto path = NodeIdsList{0, 1, 1, 1, 3, 3};
        ASSERT_VALID_PATH(graph, path);
    }
}

TEST(GraphTest, InvalidPath) {
    auto nodes = Nodes{{0, Node(0)}, {1, Node(1)}, {2, Node(2)}, {3, Node(3)}};
    auto edges = Edges{Edge(0, 1), Edge(1, 3), Edge(3, 2), Edge(2, 0)};
    auto graph = Graph(std::move(nodes), std::move(edges));
    {
        auto path = NodeIdsList{10};
        ASSERT_INVALID_PATH(graph, path);
    }
    {
        auto path = NodeIdsList{1, 0};
        ASSERT_INVALID_PATH(graph, path);
    }
    {
        auto path = NodeIdsList{0, 1, 2};
        ASSERT_INVALID_PATH(graph, path);
    }
}