#include "context/context.h"

#include <utility>

namespace mapf::simulator {

Context::Context() : event_bus{nullptr}, current_time{0.0} {}

Context::Context(EventBusPtr event_bus) : event_bus{std::move(event_bus)}, current_time{0.0} {}

}  // namespace mapf::simulator