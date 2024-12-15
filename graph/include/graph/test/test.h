#pragma once

#include <gtest/gtest.h>

#include "graph/graph.h"

namespace mapf::graph {

::testing::AssertionResult IsValidPath(const Graph& graph, const NodeIdsList& path) {
    for (const auto& node_id : path) {
        auto it = graph.nodes.find(node_id);
        if (it == graph.nodes.end()) {
            return ::testing::AssertionFailure()
                   << "node " << node_id << " is not presented in graph";
        }
        if (it->second.id != node_id) {
            return ::testing::AssertionFailure()
                   << "(" << it->second.id << ", " << it->second.pos << ").id" << " != " << node_id;
        }
    }
    for (size_t i = 0; i + 1 < path.size(); ++i) {
        if (path[i] == path[i + 1]) {
            continue;
        }
        if (!graph.GetNeighbours(path[i]).contains(path[i + 1])) {
            return ::testing::AssertionFailure()
                   << "edge " << path[i] << "->" << path[i + 1] << " is not presented in graph";
        }
    }
    return ::testing::AssertionSuccess();
}

::testing::AssertionResult IsInvalidPath(const Graph& graph, const NodeIdsList& path) {
    for (const auto& node_id : path) {
        auto it = graph.nodes.find(node_id);
        if (it == graph.nodes.end()) {
            return ::testing::AssertionSuccess();
        }
        if (it->second.id != node_id) {
            return ::testing::AssertionSuccess();
        }
    }
    for (size_t i = 0; i + 1 < path.size(); ++i) {
        if (path[i] == path[i + 1]) {
            continue;
        }
        if (!graph.GetNeighbours(path[i]).contains(path[i + 1])) {
            return ::testing::AssertionSuccess();
        }
    }
    auto verdict = ::testing::AssertionFailure() << "path: ";
    for (size_t i = 0; const auto& node_id : path) {
        verdict << node_id << (++i != path.size() ? ", " : " ");
    }
    verdict << "is valid";
    return verdict;
}

}  // namespace mapf::graph

#define ASSERT_VALID_PATH(graph, path) EXPECT_TRUE(::mapf::graph::IsValidPath(graph, path))
#define ASSERT_INVALID_PATH(graph, path) EXPECT_TRUE(::mapf::graph::IsInvalidPath(graph, path))