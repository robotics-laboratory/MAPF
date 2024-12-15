#include "event/event_bus.h"

#include <utility>

namespace mapf::simulator {

EventBus::EventBus(EventQueuePtr event_queue) : event_queue_{std::move(event_queue)} {}

}  // namespace mapf::simulator