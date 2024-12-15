#pragma once

#include "messages/message.h"

#include "models/models.h"

namespace mapf::simulator {

struct ResponseAgentStatesMessage final : public Message {
    ResponseAgentStatesMessage(models::AgentStatesPtr agent_states, bool all_finished = false);

    models::AgentStatesPtr agent_states;
    bool all_finished;
};

}  // namespace mapf::simulator