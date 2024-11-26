#pragma once

#include "simulator/event_queue.h"

#include <memory>

namespace mapf::simulator {

struct Context {
    Context();

    EventQueuePtr event_queue;
    double current_time = 0.0;
};

using ContextPtr = std::shared_ptr<Context>;

}  // namespace mapf::simulator