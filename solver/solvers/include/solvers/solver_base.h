#pragma once

#include "graph/graph.h"
#include "models/models.h"

#include <memory>

namespace mapf::solver {

struct SolverBase {
    models::proto::MAPFSolution FindProtoSolution(
        const models::proto::MAPFProblem& mapf_problem) const;

    models::proto::MAPFSolution operator()(const models::proto::MAPFProblem& mapf_problem) const;

    virtual models::MAPFSolution FindSolution(const models::MAPFProblem& mapf_problem) const = 0;
};

using SolverPtr = std::shared_ptr<SolverBase>;

}  // namespace mapf::solver