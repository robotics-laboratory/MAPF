#include "messages/agent_move_message.h"

namespace mapf::simulator {

AgentMoveMessage::AgentMoveMessage(
    uint32_t plan_version, models::AgentId agent_id, uint32_t plan_step) :
    plan_version{plan_version}, agent_id{std::move(agent_id)}, plan_step{plan_step} {}

}  // namespace mapf::simulator