#pragma once

#include "graph/graph.h"
#include "simulator/arctor.h"
#include "simulator/message.h"

namespace mapf::simulator {

using AgentId = uint32_t;

class AgentActor final : public Actor {
  public:
    AgentActor(
        ContextPtr context, AgentId agent_id, graph::GraphPtr graph, graph::Endpoints endpoints);

    const AgentId& GetAgentId() const;

    const graph::NodeId& GetCurrentNodeId() const;

    bool HasReachedGoal() const;

    AgentActor& ReceiveMessage(MessagePtr message) final;

  private:
    AgentActor& ScheduleNextMove();

    AgentId agent_id_;
    graph::GraphPtr graph_;
    graph::Endpoints endpoints_;
    graph::NodeIds plan_;
    size_t plan_progress_;
    graph::NodeId current_node_id_;
    double speed_;
};

}  // namespace mapf::simulator