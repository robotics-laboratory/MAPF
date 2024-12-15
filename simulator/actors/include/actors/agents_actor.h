#pragma once

#include "actors/actor.h"

#include "graph/graph.h"

#include "messages/agent_move_message.h"
#include "messages/mapf_solution_message.h"
#include "messages/message.h"
#include "messages/request_agent_states_message.h"

#include "models/models.h"

#include <memory>
#include <unordered_map>

namespace mapf::simulator {

class AgentsActor final : public Actor {
  public:
    AgentsActor(ContextPtr context, models::MAPFProblemPtr mapf_problem);

    AgentsActor& OnStart() final;

    const models::AgentState& GetAgentState(const models::AgentId& agent_id) const;

  private:
    bool HasReachedGoal(const models::AgentId& agent_id) const;

    void HandleAgentMoveMessage(std::shared_ptr<AgentMoveMessage> agent_move_message);

    void HandleMAPFSolutionMessage(std::shared_ptr<MAPFSolutionMessage> mapf_solution_message);

    void HandleRequestAgentStatesMessage(
        std::shared_ptr<RequestAgentStatesMessage> request_agent_states_message);

    void ScheduleNextMove(const models::AgentId& agent_id);

    models::MAPFProblemPtr mapf_problem_;
    struct VersionedMapfSolution {
        uint32_t version;
        models::MAPFSolutionPtr mapf_solution;
    } versioned_mapf_solution_;
    models::AgentStatesPtr agent_states_;
    std::unordered_map<models::AgentId, size_t> plan_progresses_;
};

}  // namespace mapf::simulator