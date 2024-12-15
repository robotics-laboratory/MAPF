#pragma once

#include "event/event.h"

#include <memory>
#include <queue>
#include <vector>

namespace mapf::simulator {

class EventQueue {
  public:
    EventQueue& Push(EventPtr event);

    bool Empty() const;

    EventPtr Extract();

  private:
    struct EventCompare {
        bool operator()(const EventPtr& lhs, const EventPtr& rhs) const;
    };

    std::priority_queue<EventPtr, std::vector<EventPtr>, EventCompare> event_queue_;
};

using EventQueuePtr = std::shared_ptr<EventQueue>;

}  // namespace mapf::simulator