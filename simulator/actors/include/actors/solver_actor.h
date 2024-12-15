#pragma once

#include "actors/actor.h"

#include "messages/mapf_problem_message.h"

#include "solvers/solver_base.h"

#include <memory>

namespace mapf::simulator {

class SolverActor final : public Actor {
  public:
    SolverActor(ContextPtr context, solver::SolverPtr solver);

  private:
    void HandleMAPFProblemMessage(std::shared_ptr<MAPFProblemMessage> mapf_problem_message);

    solver::SolverPtr solver_;
};

}  // namespace mapf::simulator