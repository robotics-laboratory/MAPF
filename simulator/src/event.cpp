#include "simulator/event.h"

namespace mapf::simulator {

Event::Event(double timestamp, std::weak_ptr<Actor> target_actor, MessagePtr message) :
    timestamp{timestamp}, target_actor{std::move(target_actor)}, message{std::move(message)} {}

}  // namespace mapf::simulator