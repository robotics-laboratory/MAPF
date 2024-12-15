#include "messages/response_agent_states_message.h"

#include <utility>

namespace mapf::simulator {

ResponseAgentStatesMessage::ResponseAgentStatesMessage(
    models::AgentStatesPtr agent_states, bool all_finished) :
    agent_states{std::move(agent_states)}, all_finished{all_finished} {}

}  // namespace mapf::simulator