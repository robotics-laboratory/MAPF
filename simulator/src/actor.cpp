#include "simulator/arctor.h"

#include <utility>

namespace mapf::simulator {

Actor::Actor(ContextPtr context) : context_{std::move(context)} {}

Actor& Actor::SendMessage(double event_time, ActorPtr target_actor, MessagePtr message) {
    context_->event_queue->Push(
        std::make_shared<Event>(event_time, std::move(target_actor), std::move(message)));
    return *this;
}

}  // namespace mapf::simulator