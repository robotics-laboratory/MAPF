#include "event/event.h"

#include <utility>

namespace mapf::simulator {

Event::Event(double timestamp, Action action) : timestamp{timestamp}, action{std::move(action)} {}

}  // namespace mapf::simulator