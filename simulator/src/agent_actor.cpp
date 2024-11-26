#include "simulator/agent_actor.h"
#include "simulator/agent_move_message.h"

namespace mapf::simulator {

AgentActor::AgentActor(
    ContextPtr context, AgentId agent_id, graph::GraphPtr graph, graph::Endpoints endpoints) :
    Actor{std::move(context)},
    agent_id_{std::move(agent_id)},
    graph_{std::move(graph)},
    endpoints_{std::move(endpoints)},
    current_node_id_{endpoints_.from_node_id},
    speed_{1.0} {}

const AgentId& AgentActor::GetAgentId() const { return agent_id_; }

const graph::NodeId& AgentActor::GetCurrentNodeId() const { return current_node_id_; }

bool AgentActor::HasReachedGoal() const { return GetCurrentNodeId() == endpoints_.to_node_id; }

AgentActor& AgentActor::ReceiveMessage(MessagePtr message) {
    if (message->type() == typeid(AgentMoveMessage)) {
        auto move_message = std::static_pointer_cast<AgentMoveMessage>(std::move(message));
        current_node_id_ = move_message->next_node;
        ++plan_progress_;
        ScheduleNextMove();
        return *this;
    }
    return *this;
}

AgentActor& AgentActor::ScheduleNextMove() {
    if (HasReachedGoal()) {
        return *this;
    }
    if (plan_progress_ >= plan_.size()) {
        return *this;
    }
    const graph::NodeId& next_node_id = plan_[plan_progress_ + 1];
    double distance = (graph_->nodes[next_node_id].pos - graph_->nodes[current_node_id_].pos).Len();
    double move_time = distance / speed_;
    double event_time = context_->current_time + move_time;
    auto message = std::make_shared<AgentMoveMessage>(next_node_id);
    SendMessage(event_time, shared_from_this(), std::move(message));
    return *this;
}

}  // namespace mapf::simulator