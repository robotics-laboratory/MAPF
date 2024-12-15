#pragma once

#include "models/models.h"

#include "graph/test/test.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <unordered_map>

namespace mapf::models {

::testing::AssertionResult IsValidSolution(
    const MAPFProblem& mapf_problem, const MAPFSolution& mapf_solution) {
    if (mapf_solution.agent_paths.empty()) {
        return ::testing::AssertionSuccess();
    }
    auto total_steps = mapf_solution.agent_paths.begin()->second.path.size();
    if (!std::all_of(
            mapf_solution.agent_paths.begin(),
            mapf_solution.agent_paths.end(),
            [&total_steps](const auto& agent_path) {
                return agent_path.second.path.size() == total_steps;
            })) {
        return ::testing::AssertionFailure() << "agents paths differs in steps count";
    }
    for (const auto& [agent_id, agent_path] : mapf_solution.agent_paths) {
        const auto& from_node_id = mapf_problem.agent_tasks.at(agent_id).endpoints.from_node_id;
        const auto& to_node_id = mapf_problem.agent_tasks.at(agent_id).endpoints.to_node_id;
        if (from_node_id != agent_path.path.front()) {
            return ::testing::AssertionFailure() << "from_node_id " << agent_path.path.front()
                                                 << " != " << from_node_id << " for " << agent_id;
        }
        if (to_node_id != agent_path.path.back()) {
            return ::testing::AssertionFailure() << "to_node_id " << agent_path.path.back()
                                                 << " != " << to_node_id << " for " << agent_id;
        }
    }
    for (const auto& [_, agent_path] : mapf_solution.agent_paths) {
        auto is_valid_path = graph::IsValidPath(mapf_problem.graph, agent_path.path);
        if (!is_valid_path) {
            return ::testing::AssertionFailure() << is_valid_path.message();
        }
    }
    std::unordered_map<graph::NodeId, models::AgentId> used;
    for (size_t current_step = 0; current_step < total_steps; ++current_step) {
        used.clear();
        for (const auto& [agent_id, agent_path] : mapf_solution.agent_paths) {
            if (used.contains(agent_path.path[current_step])) {
                return ::testing::AssertionFailure()
                       << agent_path.path[current_step] << " is occupied by " << agent_id << " and "
                       << used[agent_path.path[current_step]] << " on step " << current_step;
            }
            used[agent_path.path[current_step]] = agent_id;
        }
    }
    for (size_t current_step = 0; current_step + 1 < total_steps; ++current_step) {
        for (const auto& [_, agent_path_i] : mapf_solution.agent_paths) {
            for (const auto& [_, agent_path_j] : mapf_solution.agent_paths) {
                if (agent_path_i.agent_id == agent_path_j.agent_id) {
                    continue;
                }
                if (agent_path_i.path[current_step] == agent_path_j.path[current_step + 1]
                    && agent_path_i.path[current_step + 1] == agent_path_j.path[current_step + 1]) {
                    return ::testing::AssertionFailure()
                           << agent_path_i.agent_id << " and " << agent_path_j.agent_id
                           << " have swapping conflict";
                }
            }
        }
    }
    return ::testing::AssertionSuccess();
}

::testing::AssertionResult IsInvalidSolution(
    const MAPFProblem& mapf_problem, const MAPFSolution& mapf_solution) {
    if (mapf_solution.agent_paths.empty()) {
        return ::testing::AssertionFailure() << "empty solution";
    }
    auto total_steps = mapf_solution.agent_paths.begin()->second.path.size();
    if (!std::all_of(
            mapf_solution.agent_paths.begin(),
            mapf_solution.agent_paths.end(),
            [&total_steps](const auto& agent_path) {
                return agent_path.second.path.size() == total_steps;
            })) {
        return ::testing::AssertionSuccess();
    }
    for (const auto& [agent_id, agent_path] : mapf_solution.agent_paths) {
        const auto& from_node_id = mapf_problem.agent_tasks.at(agent_id).endpoints.from_node_id;
        const auto& to_node_id = mapf_problem.agent_tasks.at(agent_id).endpoints.to_node_id;
        if (from_node_id != agent_path.path.front()) {
            return ::testing::AssertionSuccess();
        }
        if (to_node_id != agent_path.path.back()) {
            return ::testing::AssertionSuccess();
        }
    }
    for (const auto& [_, agent_path] : mapf_solution.agent_paths) {
        auto is_valid_path = graph::IsValidPath(mapf_problem.graph, agent_path.path);
        if (!is_valid_path) {
            return ::testing::AssertionSuccess();
        }
    }
    std::unordered_map<graph::NodeId, models::AgentId> used;
    for (size_t current_step = 0; current_step < total_steps; ++current_step) {
        used.clear();
        for (const auto& [agent_id, agent_path] : mapf_solution.agent_paths) {
            if (used.contains(agent_path.path[current_step])) {
                return ::testing::AssertionSuccess();
            }
            used[agent_path.path[current_step]] = agent_id;
        }
    }
    for (size_t current_step = 0; current_step + 1 < total_steps; ++current_step) {
        for (const auto& [_, agent_path_i] : mapf_solution.agent_paths) {
            for (const auto& [_, agent_path_j] : mapf_solution.agent_paths) {
                if (agent_path_i.agent_id == agent_path_j.agent_id) {
                    continue;
                }
                if (agent_path_i.path[current_step] == agent_path_j.path[current_step + 1]
                    && agent_path_i.path[current_step + 1] == agent_path_j.path[current_step + 1]) {
                    return ::testing::AssertionSuccess();
                }
            }
        }
    }
    return ::testing::AssertionFailure() << "solution is valid";
}

}  // namespace mapf::models

#define ASSERT_VALID_SOLUTION(problem, solution) \
    EXPECT_TRUE(::mapf::models::IsValidSolution(problem, solution))
#define ASSERT_INVALID_SOLUTION(problem, solution) \
    EXPECT_TRUE(::mapf::models::IsInvalidSolution(problem, solution))