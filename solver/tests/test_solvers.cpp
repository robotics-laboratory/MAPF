#include "solvers/bfs_solver.h"

#include "graph/graph.h"

#include "models/models.h"
#include "models/test/test.h"

#include <gtest/gtest.h>

using namespace mapf::graph;
using namespace mapf::models;
using namespace mapf::solver;

class MAPFProblemTest1 : public testing::Test {
  protected:
    MAPFProblemTest1() {
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
        problem = MAPFProblem({std::move(graph), {{0, {0, {0, 3}}}, {1, {1, {3, 0}}}}});
    }

    MAPFProblem problem;
};

class MAPFProblemTest2 : public testing::Test {
  protected:
    MAPFProblemTest2() {
        auto nodes = Nodes{{0, Node(0)}, {1, Node(1)}, {2, Node(2)}, {3, Node(3)}};
        auto edges = Edges{Edge(0, 1), Edge(1, 0), Edge(1, 2), Edge(2, 1), Edge(1, 3), Edge(3, 1)};
        auto graph = Graph(std::move(nodes), std::move(edges));
        problem = MAPFProblem({std::move(graph), {{0, {0, {0, 2}}}, {1, {1, {2, 0}}}}});
    }

    MAPFProblem problem;
};

TEST_F(MAPFProblemTest1, BFSSolver) {
    auto solver = BFSSolver();
    ASSERT_VALID_SOLUTION(problem, solver.FindSolution(problem));
}

TEST_F(MAPFProblemTest2, BFSSolver) {
    auto solver = BFSSolver();
    ASSERT_VALID_SOLUTION(problem, solver.FindSolution(problem));
}