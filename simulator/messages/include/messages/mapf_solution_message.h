#pragma once

#include "messages/message.h"

#include "models/models.h"

namespace mapf::simulator {

struct MAPFSolutionMessage final : public Message {
    MAPFSolutionMessage(models::MAPFSolutionPtr mapf_solution);

    models::MAPFSolutionPtr mapf_solution;
};

}  // namespace mapf::simulator