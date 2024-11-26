#pragma once

#include <memory>

#include "simulator/message.h"

namespace mapf::simulator {

class Actor;

struct Event {
    Event(double timestamp, std::weak_ptr<Actor> target_actor, MessagePtr message);

    double timestamp;
    std::weak_ptr<Actor> target_actor;
    MessagePtr message;
};

using EventPtr = std::shared_ptr<Event>;

}  // namespace mapf::simulator