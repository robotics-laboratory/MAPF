#pragma once

#include "context/context.h"

#include "messages/message.h"

#include <memory>

namespace mapf::simulator {

class Actor;

using ActorPtr = std::shared_ptr<Actor>;
using ActorRef = std::weak_ptr<Actor>;

class Actor : public std::enable_shared_from_this<Actor> {
  public:
    explicit Actor(ContextPtr context);

    virtual ~Actor() = default;

    ActorRef GetActorRef();

    virtual Actor& OnStart();

    virtual Actor& OnFinish();

  protected:
    ContextPtr context_;
};

}  // namespace mapf::simulator