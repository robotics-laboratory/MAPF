#pragma once

#include "messages/message.h"

#include "models/models.h"

namespace mapf::simulator {

struct MAPFProblemMessage final : public Message {
    MAPFProblemMessage(models::MAPFProblemPtr mapf_problem);

    models::MAPFProblemPtr mapf_problem;
};

}  // namespace mapf::simulator