#include "simulator/event_queue.h"

#include <utility>

namespace mapf::simulator {

EventQueue& EventQueue::Push(EventPtr event) {
    event_queue_.push(std::move(event));
    return *this;
}

bool EventQueue::Empty() const { return event_queue_.empty(); }

EventPtr EventQueue::Extract() {
    if (Empty()) {
        return nullptr;
    }
    auto event = event_queue_.top();
    event_queue_.pop();
    return event;
}

bool EventQueue::EventCompare::operator()(const EventPtr& lhs, const EventPtr& rhs) const {
    return lhs->timestamp > rhs->timestamp;
}

}  // namespace mapf::simulator