#include "actors/solver_actor.h"

#include "messages/mapf_problem_message.h"
#include "messages/mapf_solution_message.h"

namespace mapf::simulator {

SolverActor::SolverActor(ContextPtr context, solver::SolverPtr solver) :
    Actor{std::move(context)}, solver_{std::move(solver)} {
    context_->event_bus->Subscribe<MAPFProblemMessage>([this](MessagePtr message) {
        HandleMAPFProblemMessage(std::static_pointer_cast<MAPFProblemMessage>(std::move(message)));
    });
}

void SolverActor::HandleMAPFProblemMessage(
    std::shared_ptr<MAPFProblemMessage> mapf_problem_message) {
    if (mapf_problem_message->mapf_problem == nullptr) {
        return;
    }
    const auto& mapf_problem = *(mapf_problem_message->mapf_problem);
    auto solution_ptr = std::make_shared<models::MAPFSolution>(solver_->FindSolution(mapf_problem));
    auto solution_message = std::make_shared<MAPFSolutionMessage>(std::move(solution_ptr));
    context_->event_bus->Publish(context_->current_time, std::move(solution_message));
}

}  // namespace mapf::simulator