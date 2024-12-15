#pragma once

#include "models/proto/models.pb.h"

#include "graph/graph.h"

#include <boost/describe.hpp>
#include <boost/container_hash/hash.hpp>

#include <google/protobuf/text_format.h>

#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

namespace mapf::models {

using AgentId = uint32_t;

struct AgentState {
    AgentState() = default;

    AgentState(AgentId agent_id, graph::NodeId node_id);

    AgentState(proto::AgentState agent_state_proto);

    bool operator==(const AgentState& other) const = default;

    AgentId agent_id;
    graph::NodeId node_id;
};

BOOST_DESCRIBE_STRUCT(AgentState, (), (agent_id, node_id));

using AgentStates = std::unordered_map<AgentId, AgentState>;
using AgentStatesPtr = std::shared_ptr<AgentStates>;

struct AgentTask {
    AgentTask() = default;

    AgentTask(AgentId agent_id, graph::Endpoints endpoints);

    AgentTask(proto::AgentTask agent_task_proto);

    bool operator==(const AgentTask& other) const = default;

    AgentId agent_id;
    graph::Endpoints endpoints;
};

BOOST_DESCRIBE_STRUCT(AgentTask, (), (agent_id, endpoints));

using AgentTaskPtr = std::shared_ptr<AgentTask>;
using AgentTasks = std::unordered_map<AgentId, AgentTask>;
using AgentTasksPtr = std::shared_ptr<AgentTasks>;

struct AgentPath {
    AgentPath() = default;

    AgentPath(AgentId agent_id, graph::NodeIdsList path);

    AgentPath(proto::AgentPath agent_path_proto);

    bool operator==(const AgentPath& other) const = default;

    AgentId agent_id;
    graph::NodeIdsList path;
};

using AgentPathPtr = std::shared_ptr<AgentPath>;
using AgentPaths = std::unordered_map<AgentId, AgentPath>;
using AgentPathsPtr = std::shared_ptr<AgentPaths>;

struct MAPFProblem {
    MAPFProblem() = default;

    MAPFProblem(graph::Graph graph, AgentTasks agent_tasks);

    MAPFProblem(proto::MAPFProblem mapf_problem_proto);

    bool operator==(const MAPFProblem& other) const = default;

    graph::Graph graph;
    AgentTasks agent_tasks;
};

using MAPFProblemPtr = std::shared_ptr<MAPFProblem>;

struct MAPFSolution {
    MAPFSolution() = default;

    MAPFSolution(AgentPaths agent_paths);

    MAPFSolution(proto::MAPFSolution mapf_solution_proto);

    bool operator==(const MAPFSolution& other) const = default;

    AgentPaths agent_paths;
};

using MAPFSolutionPtr = std::shared_ptr<MAPFSolution>;

template<typename T>
T LoadProtoFromFile(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error(std::format("Failed to open file {}", path.string()));
    }
    T proto;
    if (!google::protobuf::TextFormat::ParseFromString(
            std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()),
            &proto)) {
        throw std::runtime_error(std::format("Failed to load proto from file {}", path.string()));
    }
    file.close();
    return proto;
}

template<typename T>
void SaveProtoToFile(const T& proto, const std::filesystem::path& path) {
    std::ofstream file(path, std::fstream::out | std::fstream::trunc);
    if (!file.is_open()) {
        throw std::runtime_error(std::format("Failed to open file {}", path.string()));
    }
    std::string out_str;
    if (!google::protobuf::TextFormat::PrintToString(proto, &out_str)) {
        throw std::runtime_error(std::format("Failed to save proto to file {}", path.string()));
    }
    file << out_str;
    file.close();
}

}  // namespace mapf::models

namespace std {

template<>
class hash<mapf::models::AgentStates> {
  public:
    size_t operator()(const mapf::models::AgentStates& states) const {
        return boost::hash_unordered_range(states.begin(), states.end());
    }
};

}  // namespace std