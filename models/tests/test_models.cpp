#include "models/models.h"
#include "models/test/test.h"

#include "graph/graph.h"

#include <gtest/gtest.h>

using namespace mapf::graph;
using namespace mapf::models;

TEST(ModelTest, ValidSolution) {
    auto nodes = Nodes{{0, Node(0)}, {1, Node(1)}, {2, Node(2)}, {3, Node(3)}};
    auto edges = Edges{
        Edge(0, 1),
        Edge(1, 3),
        Edge(3, 2),
        Edge(2, 0),
        Edge(1, 0),
        Edge(3, 1),
        Edge(2, 3),
        Edge(0, 2)};
    auto graph = Graph(std::move(nodes), std::move(edges));
    auto problem = MAPFProblem({graph, {{0, {0, {0, 3}}}, {1, {1, {3, 0}}}}});
    {
        auto solution = MAPFSolution({{0, {0, {0, 1, 3}}}, {1, {1, {3, 2, 0}}}});
        ASSERT_VALID_SOLUTION(problem, solution);
    }
    {
        auto solution = MAPFSolution({{0, {0, {0, 2, 3}}}, {1, {1, {3, 1, 0}}}});
        ASSERT_VALID_SOLUTION(problem, solution);
    }
    {
        auto solution = MAPFSolution({{0, {0, {0, 0, 2, 3, 3}}}, {1, {1, {3, 3, 1, 0, 0}}}});
        ASSERT_VALID_SOLUTION(problem, solution);
    }
}

TEST(ModelTest, InvalidSolution) {
    auto nodes = Nodes{{0, Node(0)}, {1, Node(1)}, {2, Node(2)}, {3, Node(3)}};
    auto edges = Edges{Edge(0, 1), Edge(1, 3), Edge(3, 2), Edge(2, 0)};
    auto graph = Graph(std::move(nodes), std::move(edges));
    auto problem = MAPFProblem({graph, {{0, {0, {0, 3}}}, {1, {1, {3, 0}}}}});
    {
        auto solution = MAPFSolution({{0, {0, {0}}}, {1, {1, {0}}}});
        ASSERT_INVALID_SOLUTION(problem, solution);
    }
    {
        auto solution = MAPFSolution({{0, {0, {0, 1, 3}}}, {1, {1, {3, 1, 0}}}});
        ASSERT_INVALID_SOLUTION(problem, solution);
    }
    {
        auto solution = MAPFSolution({{0, {0, {0, 0, 0}}}, {1, {1, {3, 3}}}});
        ASSERT_INVALID_SOLUTION(problem, solution);
    }
    {
        auto solution = MAPFSolution({{0, {0, {0, 1}}}, {1, {1, {3, 2, 0}}}});
        ASSERT_INVALID_SOLUTION(problem, solution);
    }
    {
        auto solution = MAPFSolution({{0, {0, {0, 3}}}, {1, {1, {3, 2, 0}}}});
        ASSERT_INVALID_SOLUTION(problem, solution);
    }
}
