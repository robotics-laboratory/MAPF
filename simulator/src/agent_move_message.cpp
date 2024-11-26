#include "simulator/agent_move_message.h"

namespace mapf::simulator {

AgentMoveMessage::AgentMoveMessage(graph::NodeId next_node) : next_node{next_node} {}

std::type_index AgentMoveMessage::type() const { return typeid(AgentMoveMessage); }

}  // namespace mapf::simulator