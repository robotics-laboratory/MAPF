#pragma once

#include "messages/message.h"

namespace mapf::simulator {

struct RequestAgentStatesMessage final : public Message {
    RequestAgentStatesMessage() = default;
};

}  // namespace mapf::simulator