#include "actors/agents_actor.h"

#include "messages/mapf_problem_message.h"
#include "messages/response_agent_states_message.h"

namespace mapf::simulator {

AgentsActor::AgentsActor(ContextPtr context, models::MAPFProblemPtr mapf_problem) :
    Actor{std::move(context)},
    mapf_problem_{std::move(mapf_problem)},
    versioned_mapf_solution_{.version = 0, .mapf_solution = nullptr},
    agent_states_{std::make_shared<models::AgentStates>()} {
    agent_states_->reserve(mapf_problem_->agent_tasks.size());
    plan_progresses_.reserve(mapf_problem_->agent_tasks.size());
    for (const auto& [agent_id, agent_task] : mapf_problem_->agent_tasks) {
        agent_states_->emplace(
            agent_id, models::AgentState(agent_id, agent_task.endpoints.from_node_id));
        plan_progresses_.emplace(agent_id, 0);
    }
    context_->event_bus
        ->Subscribe<AgentMoveMessage>([this](MessagePtr message) {
            HandleAgentMoveMessage(std::static_pointer_cast<AgentMoveMessage>(std::move(message)));
        })
        .Subscribe<MAPFSolutionMessage>([this](MessagePtr message) {
            HandleMAPFSolutionMessage(
                std::static_pointer_cast<MAPFSolutionMessage>(std::move(message)));
        })
        .Subscribe<RequestAgentStatesMessage>([this](MessagePtr message) {
            HandleRequestAgentStatesMessage(
                std::static_pointer_cast<RequestAgentStatesMessage>(std::move(message)));
        });
}

AgentsActor& AgentsActor::OnStart() {
    context_->event_bus->Publish(
        context_->current_time, std::make_shared<MAPFProblemMessage>(mapf_problem_));
    return *this;
}

const models::AgentState& AgentsActor::GetAgentState(const models::AgentId& agent_id) const {
    return agent_states_->at(agent_id);
}

bool AgentsActor::HasReachedGoal(const models::AgentId& agent_id) const {
    return GetAgentState(agent_id).node_id
           == mapf_problem_->agent_tasks[agent_id].endpoints.to_node_id;
}

void AgentsActor::HandleAgentMoveMessage(std::shared_ptr<AgentMoveMessage> agent_move_message) {
    if (agent_move_message->plan_version != versioned_mapf_solution_.version) {
        return;
    }
    const auto& agent_id = agent_move_message->agent_id;
    plan_progresses_[agent_id] = agent_move_message->plan_step;
    (*agent_states_)[agent_id] = models::AgentState(
        agent_id,
        versioned_mapf_solution_.mapf_solution->agent_paths[agent_id]
            .path[agent_move_message->plan_step]);
    ScheduleNextMove(agent_id);
}

void AgentsActor::HandleMAPFSolutionMessage(
    std::shared_ptr<MAPFSolutionMessage> mapf_solution_message) {
    ++versioned_mapf_solution_.version;
    versioned_mapf_solution_.mapf_solution = std::move(mapf_solution_message->mapf_solution);
    for (const auto& [agent_id, _] : mapf_problem_->agent_tasks) {
        plan_progresses_[agent_id] = 0;
        ScheduleNextMove(agent_id);
    }
}

void AgentsActor::HandleRequestAgentStatesMessage(
    std::shared_ptr<RequestAgentStatesMessage> request_agent_states_message) {
    auto agent_states = std::make_shared<models::AgentStates>();
    bool all_finished = true;
    for (const auto& [agent_id, _] : mapf_problem_->agent_tasks) {
        agent_states->emplace(agent_id, GetAgentState(agent_id));
        all_finished &= HasReachedGoal(agent_id);
    }
    context_->event_bus->Publish(
        context_->current_time,
        std::make_shared<ResponseAgentStatesMessage>(std::move(agent_states), all_finished));
}

void AgentsActor::ScheduleNextMove(const models::AgentId& agent_id) {
    if (HasReachedGoal(agent_id)) {
        return;
    }
    if (versioned_mapf_solution_.mapf_solution == nullptr) {
        return;
    }
    const auto& plan = versioned_mapf_solution_.mapf_solution->agent_paths[agent_id].path;
    if (plan_progresses_[agent_id] >= plan.size()) {
        return;
    }
    double move_time = 1;  // All agents speeds equals 1 for now
    auto message = std::make_shared<AgentMoveMessage>(
        versioned_mapf_solution_.version, agent_id, plan_progresses_[agent_id] + 1);
    context_->event_bus->Publish(context_->current_time + move_time, std::move(message));
}

}  // namespace mapf::simulator