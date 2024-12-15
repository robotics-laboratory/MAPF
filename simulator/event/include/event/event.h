#pragma once

#include <functional>
#include <memory>

namespace mapf::simulator {

using Action = std::function<void()>;

struct Event {
    Event(double timestamp, Action action);

    double timestamp;
    Action action;
};

using EventPtr = std::shared_ptr<Event>;

}  // namespace mapf::simulator