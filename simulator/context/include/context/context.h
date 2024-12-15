#pragma once

#include "event/event_bus.h"

#include <memory>

namespace mapf::simulator {

struct Context {
    Context();

    Context(EventBusPtr event_bus);

    EventBusPtr event_bus;
    double current_time;
};

using ContextPtr = std::shared_ptr<Context>;

}  // namespace mapf::simulator