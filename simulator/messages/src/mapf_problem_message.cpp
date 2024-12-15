#include "messages/mapf_problem_message.h"

#include <utility>

namespace mapf::simulator {

MAPFProblemMessage::MAPFProblemMessage(models::MAPFProblemPtr mapf_problem) :
    mapf_problem{std::move(mapf_problem)} {}

}  // namespace mapf::simulator