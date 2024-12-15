#include "solvers/solver_base.h"

#include "models/proto/models.pb.h"

namespace mapf::solver {

models::proto::MAPFSolution SolverBase::FindProtoSolution(
    const models::proto::MAPFProblem& mapf_problem) const {
    auto mapf_solution = this->FindSolution(models::MAPFProblem{mapf_problem});
    auto mapf_solution_proto = models::proto::MAPFSolution();
    for (const auto& [agent_id, agent_path] : mapf_solution.agent_paths) {
        auto* agent_path_proto = mapf_solution_proto.add_agent_paths();
        agent_path_proto->set_agent_id(agent_id);
        for (const auto& node_id : agent_path.path) {
            agent_path_proto->add_path(node_id);
        }
    }
    return mapf_solution_proto;
}

models::proto::MAPFSolution SolverBase::operator()(
    const models::proto::MAPFProblem& mapf_problem) const {
    return FindProtoSolution(mapf_problem);
}

}  // namespace mapf::solver