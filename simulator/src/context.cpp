#include "simulator/context.h"

namespace mapf::simulator {

Context::Context() : event_queue{std::make_shared<EventQueue>()}, current_time{0.0} {}

}