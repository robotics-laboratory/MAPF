#pragma once

#include "graph/graph.h"
#include "simulator/message.h"

namespace mapf::simulator {

struct AgentMoveMessage final : public Message {
    AgentMoveMessage(graph::NodeId next_node);

    std::type_index type() const final;

    graph::NodeId next_node;
};

}  // namespace mapf::simulator