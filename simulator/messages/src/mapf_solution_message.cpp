#include "messages/mapf_solution_message.h"

#include <utility>

namespace mapf::simulator {

MAPFSolutionMessage::MAPFSolutionMessage(models::MAPFSolutionPtr mapf_solution) :
    mapf_solution{std::move(mapf_solution)} {}

}  // namespace mapf::simulator