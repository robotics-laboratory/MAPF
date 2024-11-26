#pragma once

#include "simulator/context.h"
#include "simulator/message.h"

#include <memory>

namespace mapf::simulator {

class Actor;

using ActorPtr = std::shared_ptr<Actor>;

class Actor : public std::enable_shared_from_this<Actor> {
  public:
    explicit Actor(ContextPtr context);

    virtual ~Actor() = default;

    virtual Actor& ReceiveMessage(MessagePtr message) = 0;

  protected:
    Actor& SendMessage(double event_time, ActorPtr target_actor, MessagePtr message);

    ContextPtr context_;
};

}  // namespace mapf::simulator