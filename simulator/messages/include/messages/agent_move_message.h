#pragma once

#include "messages/message.h"

#include "models/models.h"

namespace mapf::simulator {

struct AgentMoveMessage final : public Message {
    AgentMoveMessage(uint32_t plan_version, models::AgentId agent_id, uint32_t plan_step);

    uint32_t plan_version;
    models::AgentId agent_id;
    uint32_t plan_step;
};

}  // namespace mapf::simulator